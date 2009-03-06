#include "searcher.h"
#include <string>

namespace Lux {
namespace Dist {

  DocumentSerializer Searcher::doc_serializer_;

  Searcher::Searcher(std::string service)
  : config_helper_(new ServerConfigHelper(service))
  {
    dispatchers_ = config_helper_->get_dispatchers();
  }

  Searcher::~Searcher(void)
  {

  }

  ResultSet Searcher::search(const char *query, Condition &cond)
  {
    return search(std::string(query), cond);
  }

  ResultSet Searcher::search(std::string query, Condition &cond)
  {
    // select dispatcher
    static int did = 0;
    if (did >= dispatchers_.dispatcher_size()) {
      did = 0;
    }
    Lux::Config::Dispatcher d = dispatchers_.dispatcher(did++);

    sock_t s;
    in_addr_t ip = get_ip_by_str(d.server().host().c_str());
    if (!sock_connect(s, ip, d.server().port())) {
      error_log("sock_connect() failed.");
    }
    
    if (!send_search_request(s, query, cond)) {
      error_log("send_search_request() failed.");
    }

    ResultSet rs;
    if (!recv_search_request(s, rs)) {
      error_log("recv_search_request() failed.");
    }

    close(s);
    return rs;
  }

  bool Searcher::send_search_request(sock_t s,
                                     std::string &query, Condition &cond)
  {
    uint32_t body_len = sizeof(search_options_t) + query.size();
    if (body_len > (1 << PACKET_BODY_MAX_BITS)) {
      error_log("sending too long request");
      return false;
    }

    request_header_t header = {body_len, 0, COM_DISPATCH};
    search_options_t opts;
    opts.sort_attr = cond.sort.attr_type;
    opts.sort_order = cond.sort.order_type;
    memset(opts.attr_name, 0, sizeof(opts.attr_name));
    memcpy(opts.attr_name, cond.sort.attr_name.c_str(), 
           (cond.sort.attr_name.size() > sizeof(opts.attr_name))
           ? sizeof(opts.attr_name) : cond.sort.attr_name.size());
    opts.num = cond.paging.get_num();
    opts.page = cond.paging.get_page();

    struct iovec v[3];
    v[0].iov_base = &header;
    v[0].iov_len = sizeof(header);
    v[1].iov_base = &opts;
    v[1].iov_len = sizeof(opts);
    v[2].iov_base = &query[0];
    v[2].iov_len = query.size();

    if (lut_writev(s, v, 3) < 0) {
      perror("writev failed.");
      return false; 
    }

    return true;
  }

  bool Searcher::recv_search_request(sock_t s, ResultSet &rs)
  {
    response_header_t header;
    if (lut_recv(s, (char *) &header, sizeof(header), 0) <= 0) {
      error_log("lut_recv() failed");
      return false;
    }   

    std::cout << "header.length = " << header.length << std::endl;
    std::cout << "header.comp_length = " << header.comp_length << std::endl;
    uint32_t body_size = (header.comp_length > 0) ? 
                            header.comp_length : header.length;

    char *body = NULL;
    try {
      body = new char[body_size];
      if (lut_recv(s, body, body_size, 0) <= 0) {
        throw std::runtime_error("lut_recv() failed.");
      }

      if (header.status == STATUS_ERR) {
        throw std::runtime_error("status error");
      }
     
      char *data = body;
      uint32_t size = header.length;
#ifdef USE_ZLIB
      if (header.comp_length > 0) {
        double t1 = gettimeofday_sec();
        char *uncomp = uncompress(body, body_size, &size);
        if (uncomp != NULL) {
          data = uncomp;
        } else {
          size = body_size;
        }
        double t2 = gettimeofday_sec();
        std::cout << "uncompress time: " << t2 - t1 << std::endl;
      }
#endif

      if (!deserialize_rs(rs, data, size)) {
        std::cout << "deserialize_rs failed." << std::endl;
        throw std::runtime_error("deserialize_rs() failed.");
      }

    } catch (...) {
      delete [] body;
      return false;
    }
    delete [] body;

    return true;
  }

  bool Searcher::deserialize_rs(ResultSet &rs, char *body, uint32_t len)
  {
    char *p = body;
    char *last = body + len;
    proxy_doc_result_header_t *h = (proxy_doc_result_header_t *) p;
    p += sizeof(proxy_doc_result_header_t); 

    ResultSet tmprs(h->total_num, h->base, h->num);
    bool is_failed = false; 
    for (int i = 0; i < h->num; ++i) {
      if (p >= last) {
        is_failed = true;
        break;
      }
      uint32_t *size = (uint32_t *) p;
      p += sizeof(uint32_t);
      if (p >= last) {
        is_failed = true;
        break;
      }
      std::string doc_str(p, *size);
      p += *size;
      if (p >= last) {
        is_failed = true;
        break;
      }
      Document doc = doc_serializer_.unpack((char *) doc_str.c_str());
      tmprs.add(Result(doc, *(score_t *) p));
      p += sizeof(score_t); 
    }

    if (is_failed) {
      error_log("received packet is possibly broken.");
      return false;
    }

    rs = tmprs;
    return true;
  }

}
}
