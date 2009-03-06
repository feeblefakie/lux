#include "dispatcher.h"
#include "server-config.pb.h"
#include "lux/util.h"

namespace Lux {
namespace Dist {

  Dispatcher::Dispatcher(void *p)
  : Handler(p),
    s_(new ClusterSearcher((Lux::Config::Cluster *) inf_->supp_arg))
  {}

  Dispatcher::~Dispatcher(void)
  {}

  bool Dispatcher::exec(request_header_t &header, char *body, uint32_t body_len)
  {
    switch (header.command) {
      case COM_QUIT:
      {
        is_persistent_ = false;
        exec_quit();
        is_terminating_ = true;
        break;
      }
      case COM_DISPATCH:
      {
        std::cout << "COM_DISPATCH" << std::endl;
        is_persistent_ = false;
        if (!exec_dispatch(body, body_len)) {
          error_log("exec_find() failed");
          return false;
        }
        break;
      }
      default:
        break;
    }
    return true;
  }

  bool Dispatcher::exec_dispatch(char *body, uint32_t len)
  {
    search_options_t *opts;
    opts = (search_options_t *) body;
    body += sizeof(search_options_t);

    Lux::SortCondition scond((sort_attr_t) opts->sort_attr,
                             (sort_order_t) opts->sort_order,
                             opts->attr_name);
    Lux::Paging paging(opts->num, opts->page);
    Lux::Condition cond(scond, paging);

    TmpResultSet rs;
    try {
      std::string query(body, len - sizeof(search_options_t));
      MergeResultSet mrs = s_->search_servers_and_merge(query, cond);
      rs = s_->get_docs_from_servers(mrs);
    } catch (const std::runtime_error& e) {
      error_log(e.what());
      send_error(CLUSTER_ERROR);
      return false;
    } catch (...) {
      send_error(RUNTIME_ERROR);
      return false;
    }

    char *data;
    uint32_t size;
    if (!serialize_rs(rs, &data, &size)) {
      send_error(CLUSTER_ERROR);
      return false;
    }

    send_packets(data, size, STATUS_RS);

    delete [] data;
    
    return true;
  }

  bool Dispatcher::serialize_rs(TmpResultSet &rs, char **data, uint32_t *size)
  {
    Paging paging = rs.paging;
    ClusterResultSet crs = rs.crs;

    *size = sizeof(proxy_doc_result_header_t) // paging
            + crs.size() * sizeof(uint32_t) // doc size
            + rs.total_doc_size  // documents
            + crs.size() * sizeof(score_t); // documents' score

    char *tmp = new char[*size];
    char *p = tmp;

    proxy_doc_result_header_t header;
    header.total_num = paging.get_total_num();
    header.base = paging.get_base();
    header.num = paging.get_num();

    memcpy(p, &header, sizeof(header));
    p += sizeof(header);

    for (int i = 0; i < header.num; ++i) {
      char *doc_p = (char *) (crs[i].doc.c_str());
      uint32_t doc_size = crs[i].doc.size();
      memcpy(p, &doc_size, sizeof(uint32_t));
      p += sizeof(uint32_t);
      memcpy(p, doc_p, doc_size * sizeof(char));
      p += doc_size * sizeof(char);
      memcpy(p, &(crs[i].score), sizeof(score_t));
      p += sizeof(score_t);
    }

    *data = tmp;

    return true;
  }

}
}
