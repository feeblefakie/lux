#include "cluster_searcher.h"
#include "lux/StorageInterface/Sorter.h"
#include <string>
#include <vector>
#include <stdexcept>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

namespace Lux {
namespace Dist {

  DocumentSerializer ClusterSearcher::doc_serializer_;

  // spefifying lux.conf in the argument ? (/etc/lux.conf in default)
  ClusterSearcher::ClusterSearcher(Lux::Config::Cluster *cluster_config)
  try
  : num_servers_(cluster_config->server_size())
  {
    servers_ = new server_inf_t[num_servers_];
    clients_ = new client_t[num_servers_];
    args_ = new arg_t[num_servers_];

    for (int i = 0; i < cluster_config->server_size(); ++i) {
      Lux::Config::Server server = cluster_config->server(i);

      servers_[i].ip = get_ip_by_str(server.host().c_str());
      if (servers_[i].ip == 0) {
        throw std::runtime_error("can't get an address.");
      }
      servers_[i].socket = -1;
      servers_[i].port = server.port();
      servers_[i].id = i;

      clients_[i].status = INITIALIZED;
      clients_[i].resp = new response_t;
      clients_[i].resp->h = new response_header_t;
      clients_[i].resp->h_received = 0;
      clients_[i].resp->b = NULL;
      clients_[i].resp->b_received = 0;
      clients_[i].resp->b_length = 0;
    }

    ebase_ = event_base_new();
  }
  catch (...) {
    release_all();
    throw;
  }

  ClusterSearcher::~ClusterSearcher(void)
  {
    release_all();
  }

  MergeResultSet ClusterSearcher::search_servers_and_merge(const char *query, Condition &cond)
  {
    return search_servers_and_merge(std::string(query), cond);
  }

  MergeResultSet ClusterSearcher::search_servers_and_merge(const std::string &query, Condition &cond) {
    query_ = query;
    cond_ = cond;

    MergeResultSet mrs;
    try {
      if (!search_indices()) {
        throw std::runtime_error("search_indicies() failed.");
      }

      if (!merge_results(mrs)) {
        throw std::runtime_error("merge_results() failed.");
      }

      uint32_t num_sorted = cond_.paging.get_base() + cond_.paging.get_num();
      Sorter<MergeResult> sorter(cond_.sort, num_sorted);
      sorter.sort(mrs);

    } catch (...) {
      empty_clients();
      if (cond_.sort.attr_type != SORT_SCORE) { clear_mrs(mrs); }
      throw;
    }

    empty_clients();
    if (cond_.sort.attr_type != SORT_SCORE) { clear_mrs(mrs); }

    return mrs;
  }

  TmpResultSet ClusterSearcher::get_docs_from_servers(MergeResultSet &mrs)
  {
    TmpResultSet trs;
    try {
      if (!collect_doc_by_server(mrs)) {
        throw std::runtime_error("collect_doc_by_server() failed.");
      }

      if (!search_docs()) {
        throw std::runtime_error("search_docs() failed.");
      }

      trs = get_tmp_resultset(mrs);

   } catch (...) {
      empty_clients();
      throw;
    }
    empty_clients();

    return trs;
  }

  ResultSet ClusterSearcher::search(const char *query, Condition &cond)
  {
    return search(std::string(query), cond);
  }

  ResultSet ClusterSearcher::search(std::string query, Condition &cond)
  {
    query_ = query;
    cond_ = cond;

    ResultSet rs;
    MergeResultSet mrs;
    try {
      if (!search_indices()) {
        error_log("search_indicies() failed.");
        return ResultSet();
      }

      if (!merge_results(mrs)) {
        error_log("merge_results() failed.");
        return ResultSet();
      }
      // clean up received data
      empty_clients();
      
      uint32_t num_sorted = cond_.paging.get_base() + cond_.paging.get_num();
      Sorter<MergeResult> sorter(cond_.sort, num_sorted);
      sorter.sort(mrs);

      if (!collect_doc_by_server(mrs)) {
        throw std::runtime_error("collect_doc_by_server() failed.");
      }

      if (!search_docs()) {
        throw std::runtime_error("search_docs() failed.");
      }

      rs = get_resultset(mrs);

    } catch (...) {
      empty_clients();
      if (cond_.sort.attr_type != SORT_SCORE) { clear_mrs(mrs); }
      return ResultSet();
    }

    // clean up received data
    empty_clients();
    if (cond_.sort.attr_type != SORT_SCORE) { clear_mrs(mrs); }

    return rs;
  }

  void ClusterSearcher::release_all(void)
  {
    for (uint32_t sid = 0; sid < num_servers_; ++sid) {
      delete clients_[sid].resp->h;
      if (clients_[sid].resp->b != NULL) {
        delete [] clients_[sid].resp->b;
      }
      delete clients_[sid].resp;
    }
    delete [] servers_;
    delete [] clients_;
    delete [] args_;
    event_base_free(ebase_);
  }

  void ClusterSearcher::empty_clients(void)
  {
    for (uint32_t sid = 0; sid < num_servers_; ++sid) {
      clients_[sid].status = INITIALIZED;
      clients_[sid].resp->h_received = 0;
      if (clients_[sid].resp->b != NULL) {
        delete [] clients_[sid].resp->b;
        clients_[sid].resp->b = NULL;
      }
      clients_[sid].resp->b_received = 0;
      clients_[sid].resp->b_length = 0;
    }
  }

  bool ClusterSearcher::merge_results(MergeResultSet &mrs)
  {
    uint32_t total_num = 0;
    for (uint32_t i = 0; i < num_servers_; i++) {
      client_t *cli = &(clients_[i]);
      char *p = cli->resp->b;
      cluster_idx_result_header_t *rh = (cluster_idx_result_header_t *) p;

      // check the body size
      if (sizeof(cluster_idx_result_header_t) + 
          rh->num_results * (sizeof(doc_id_t) + rh->attr_size) 
          != cli->resp->b_length) {
        error_log("response packet might be broken.");
        return false;
      }

      p += sizeof(cluster_idx_result_header_t);
      total_num += rh->total_num;

      for (int j = 0; j < rh->num_results; j++) {
        MergeResult mr;
        mr.server_id = i;
        doc_id_t *id = (doc_id_t *) p;
        mr.doc_id = *id;
        p += sizeof(doc_id_t);
        // [TODO] consider having +1 size
        if (cond_.sort.attr_type == SORT_SCORE) {
          mr.score = *(score_t *) p;
        } else {
          mr.attr = new char[rh->attr_size];
          memcpy(mr.attr, p, rh->attr_size);
        }
        p += rh->attr_size; 
        mrs.push_back(mr);
      }
    }

    // [TODO] better not do this here ?
    cond_.paging.set_total_num(total_num);
    // the same calculation is done in both server and client
    // paging.calc() must be called only once
    cond_.paging.calc();

    return true;
  }

  bool ClusterSearcher::collect_doc_by_server(MergeResultSet &mrs)
  {
    dloc_.clear();
    for (uint32_t i = 0; i < num_servers_; ++i) {
      std::deque<doc_id_t> docs_by_server;
      dloc_.push_back(docs_by_server);
    }
    for (uint32_t i = cond_.paging.get_base();
         i < cond_.paging.get_base() + cond_.paging.get_num(); ++i) {
      dloc_[mrs[i].server_id].push_back(mrs[i].doc_id);
    }

    return true;
  }

  bool ClusterSearcher::nonblocking_connect(uint32_t sid, command_t command)
  {
    server_inf_t *serv = &(servers_[sid]);
    client_t *cli = &(clients_[sid]);
    arg_t *arg = &(args_[sid]);

    if ((serv->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return false;
    }
    int on = 1;
    setsockopt(serv->socket, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    set_nonblocking(serv->socket);
    struct sockaddr_in server;
    init_sockaddr(server, serv->ip, serv->port);

    int n;
    if ((n = lut_connect(serv->socket, 
            (struct sockaddr *) &server, sizeof(server))) < 0) {
      if (errno != EINPROGRESS) {
        close(serv->socket);
        return false;
      }
      cli->status = CONNECTING;
      
      arg->ins = this;
      arg->serv = serv;
      arg->cli = cli;
      arg->command = command;
      event_set(&(arg->e), serv->socket, EV_READ | EV_WRITE, ClusterSearcher::run, arg);
      event_base_set(ebase_, &(arg->e));
      event_add(&(arg->e), NULL);
    } else if (n >= 0) {
      cli->status = READING;
    }

    return true;
  }

  bool ClusterSearcher::search_indices(void)
  {
    command_t command = COM_FIND;
    empty_clients();
    for (uint32_t sid = 0; sid < num_servers_; ++sid) {
      if (!nonblocking_connect(sid, command)) {
        error_log("nonblocking_connect() failed.");
        return false;
      }
      if (clients_[sid].status == READING) {
        // connection established right away
        if (!send_request(&(servers_[sid]), command)) {
          error_log("send_request() failed.");
          return false;
        }
      }
    }
    event_base_dispatch(ebase_);

    for (uint32_t sid = 0; sid < num_servers_; ++sid) {
      if (clients_[sid].status != DONE) { // check client status
        close(servers_[sid].socket);
        return false;
      }
      if (clients_[sid].resp->h->status == STATUS_ERR) { // check server status
        // [TODO] check error code
        close(servers_[sid].socket);
        return false;
      }
    }

    return true;
  }

  bool ClusterSearcher::search_docs(void)
  {
    command_t command = COM_GET_DOCS;
    empty_clients();
    for (uint32_t sid = 0; sid < dloc_.size(); ++sid) {
      // no access to the server having no matched docs
      if (dloc_[sid].empty()) { continue; }
      if (!nonblocking_connect(sid, command)) {
        error_log("nonblocking_connect() failed.");
        return false;
      }
      if (clients_[sid].status == READING) {
        // connection established right away
        if (!send_request(&(servers_[sid]), command)) {
          error_log("send_request() failed.");
          close(servers_[sid].socket);
          return false;
        }
      }
    }
    event_base_dispatch(ebase_);

    for (uint32_t sid = 0; sid < num_servers_; ++sid) {
      if (dloc_[sid].empty()) { continue; }
      if (clients_[sid].status != DONE) {
        close(servers_[sid].socket);
        return false;
      }
    }
    
    return true;
  }

  void ClusterSearcher::set_nonblocking(int &socket)
  {
    int flags = fcntl(socket, F_GETFL, 0);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
  }

  void ClusterSearcher::init_sockaddr(struct sockaddr_in &addr,
                               in_addr_t ip, uint16_t port)
  {
    memset((char *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ip;
    addr.sin_port = htons(port);
  }
  
  void ClusterSearcher::run(int fd, short event, void *arg)
  {
    (((arg_t *) arg)->ins)->handler(fd, event, arg);
  }

  void ClusterSearcher::handler(int fd, short event, void *arg)
  {
    server_inf_t *serv = (server_inf_t *) ((arg_t *) arg)->serv;
    client_t *cli = (client_t *) ((arg_t *) arg)->cli;
    struct event *e = &(((arg_t *) arg)->e);

    if (cli->status & CONNECTING) {
      int error;
      int n = sizeof(error);
      if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *) &n) < 0
          || error != 0) {
        error_log("nonblocking connect failed");
        cli->status = ERROR;
        close(fd);
        return;
      }

      if (!send_request(serv, ((arg_t *) arg)->command)) {
        close(fd);
        return;
      }

      cli->status = READING;
      event_set(e, fd, EV_READ, ClusterSearcher::run, arg);
      event_base_set(ebase_, e);
      event_add(e, NULL);
    } else if (cli->status & READING) {
      int n = recv_search_response(fd, cli->resp);
      if (n < 0) {
        error_log("recv_search_response failed.");
        cli->status = ERROR;
        close(fd);
      } else if (n == 0) {
        if (cli->resp->b_received != cli->resp->b_length) {
          error_log("received the data failed or something wrong in server.");
          cli->status = ERROR;
        } else {
          cli->status = DONE;
#ifdef USE_ZLIB
          if (cli->resp->h->comp_length > 0) {
            uint32_t org_size = cli->resp->h->length;
            double t1 = gettimeofday_sec();
            char *uncomp = uncompress(cli->resp->b, 
                                      cli->resp->b_length, &org_size);
            if (uncomp == NULL) {
              error_log("uncompress() failed.");
              cli->status = ERROR;
            } else {
              delete [] cli->resp->b;
              cli->resp->b = uncomp;
              // set original size (uncompressed size)
              cli->resp->b_length = cli->resp->h->length;
            }
            double t2 = gettimeofday_sec();
          }
#endif
        }
        close(fd);
      } else {
        event_set(e, fd, EV_READ, ClusterSearcher::run, arg);
        event_base_set(ebase_, e);
        event_add(e, NULL);
      }
    }
  }

  int ClusterSearcher::recv_search_response(int fd, response_t *resp)
  {
    int n;
    // check if the header is already read
    if (resp->h_received == sizeof(response_header_t)) {
      if (resp->b == NULL) {
        resp->b_length = resp->h->length;
        if (resp->h->comp_length > 0) {
          resp->b_length = resp->h->comp_length;
        }
        resp->b = new char[resp->b_length];
      }
      SAFE_SYSCALL(n, recv(fd, resp->b + resp->b_received, 
                           resp->b_length - resp->b_received, 0));
      if (n < 0) { perror("recv"); }
      if (n <= 0) { return n; }
      resp->b_received += n;
    } else { // header is not read completely
      SAFE_SYSCALL(n, recv(fd, resp->h + resp->h_received, 
                           sizeof(response_header_t) - resp->h_received, 0));
      if (n < 0) { perror("recv"); }
      if (n <= 0) { return n; }
      resp->h_received += n;
    }

    // -1: error
    //  0: connection closed in server side (must received all or not)
    //  1: success (client received some data successfully)
    return 1;
  }

  bool ClusterSearcher::send_request(server_inf_t *serv, command_t command)
  {
    bool res = false;
    // [NOTICE] might be better to be polymorphismic, 
    // but the cases are fixed, so it's not a big deal.
    switch (command) {
      case COM_FIND:
        res = send_search_request(serv);
        break;
      case COM_GET_DOCS:
        res = send_getdocs_request(serv);
        break;
      default:
        break;
    }
    return res;
  }

  bool ClusterSearcher::send_search_request(server_inf_t *serv)
  {
    uint32_t body_len = sizeof(search_options_t) + query_.size();
    if (body_len > (1 << PACKET_BODY_MAX_BITS)) {
      error_log("sending too long request.");
      return false;
    }

    request_header_t header = {body_len, 0, COM_FIND};
    search_options_t opts;
    opts.sort_attr = cond_.sort.attr_type;
    opts.sort_order = cond_.sort.order_type;
    memset(opts.attr_name, 0, sizeof(opts.attr_name));
    memcpy(opts.attr_name, cond_.sort.attr_name.c_str(), 
           (cond_.sort.attr_name.size() > sizeof(opts.attr_name))
           ? sizeof(opts.attr_name) : cond_.sort.attr_name.size());
    opts.num = cond_.paging.get_num() * (cond_.paging.get_page() + 1);
    opts.page = 0; // always 0 for cluster

    struct iovec v[3];
    v[0].iov_base = &header;
    v[0].iov_len = sizeof(header);
    v[1].iov_base = &opts;
    v[1].iov_len = sizeof(opts);
    v[2].iov_base = &(query_[0]);
    v[2].iov_len = query_.size();

    if (lut_writev(serv->socket, v, 3) < 0) {
      error_log("writev failed");
      return false;
    }

    return true;
  }

  bool ClusterSearcher::send_getdocs_request(server_inf_t *serv)
  {
    // [TODO] care about compression
    std::deque<doc_id_t> docs_by_server = dloc_[serv->id];
    uint32_t body_len = docs_by_server.size() * sizeof(doc_id_t);
    if (body_len > (1 << PACKET_BODY_MAX_BITS)) {
      error_log("sending too long request.");
      return false;
    }

    request_header_t header = {body_len, 0, COM_GET_DOCS};
    char *body = new char[body_len+1];
    char *p = body;
    memset(body, 0, body_len+1);
    for (uint32_t i = 0; i < docs_by_server.size(); ++i) {
      doc_id_t doc_id = docs_by_server[i];
      memcpy(p, &doc_id, sizeof(doc_id_t));
      p += sizeof(doc_id_t);
    }

    struct iovec v[2];
    v[0].iov_base = &header;
    v[0].iov_len = sizeof(header);
    v[1].iov_base = body;
    v[1].iov_len = body_len;
    if (lut_writev(serv->socket, v, 2) < 0) {
      perror("writev");
      return false;
    }

    delete [] body;

    return true;
  }

#define CHECK_PTR(p,last,flag) { \
  if (p >= last) { \
    flag = true; \
    break; \
  } \
}

  TmpResultSet ClusterSearcher::get_tmp_resultset(MergeResultSet &mrs)
  {
    bool is_failed = false;
    char *ptrs[num_servers_];
    char *lasts[num_servers_];
    for (uint32_t i = 0; i < num_servers_; ++i) {
      ptrs[i] = NULL;
      lasts[i] = NULL;
    }

    ClusterResultSet crs;
    uint32_t total_doc_size = 0;
    for (uint32_t i = cond_.paging.get_base();
         i < cond_.paging.get_base() + cond_.paging.get_num(); ++i) {
      uint32_t sid = mrs[i].server_id;
      char *p = ptrs[sid];
      if (p == NULL) {
        p = clients_[sid].resp->b;
        lasts[sid] = p + clients_[sid].resp->b_length;
      }
      CHECK_PTR(p, lasts[sid], is_failed);

      // [TODO] validation check
      doc_id_t *id = (doc_id_t *) p;
      p += sizeof(doc_id_t);
      CHECK_PTR(p, lasts[sid], is_failed);

      uint32_t *size = (uint32_t *) p;
      p += sizeof(uint32_t);
      CHECK_PTR(p, lasts[sid], is_failed);

      std::string doc_str(p, *size);
      total_doc_size += *size;
      p += *size;
      // update pointer
      ptrs[sid] = p;
      score_t score = 0;
      if (cond_.sort.attr_type == SORT_SCORE) {
        score = mrs[i].score;
      }
      crs.push_back(ClusterResult(doc_str, score));
    }

    return TmpResultSet(cond_.paging, crs, total_doc_size);
  }

  ResultSet ClusterSearcher::get_resultset(MergeResultSet &mrs)
  {
    bool is_failed = false;
    char *ptrs[num_servers_];
    char *lasts[num_servers_];
    for (uint32_t i = 0; i < num_servers_; ++i) {
      ptrs[i] = NULL;
      lasts[i] = NULL;
    }

    ResultSet rs(cond_.paging);
    for (uint32_t i = cond_.paging.get_base();
         i < cond_.paging.get_base() + cond_.paging.get_num(); ++i) {
      uint32_t sid = mrs[i].server_id;
      char *p = ptrs[sid];
      if (p == NULL) {
        p = clients_[sid].resp->b;
        lasts[sid] = p + clients_[sid].resp->b_length;
      }
      CHECK_PTR(p, lasts[sid], is_failed);

      // [TODO] validation check
      doc_id_t *id = (doc_id_t *) p;
      p += sizeof(doc_id_t);
      CHECK_PTR(p, lasts[sid], is_failed);

      uint32_t *size = (uint32_t *) p;
      p += sizeof(uint32_t);
      CHECK_PTR(p, lasts[sid], is_failed);

      std::string doc_str(p, *size);
      p += *size;
      // update pointer
      ptrs[sid] = p;
      Document doc = doc_serializer_.unpack((char *) doc_str.c_str());
      if (cond_.sort.attr_type == SORT_SCORE) {
        rs.add(Result(doc, mrs[i].score));
      } else {
        rs.add(Result(doc));
      }
    }

    if (is_failed) {
      error_log("response packet might be broken.");
      return ResultSet(); // empty ResultSet
    }
    return rs; 
  }

}
}
