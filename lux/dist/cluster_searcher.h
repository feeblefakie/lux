#ifndef LUX_DIST_CLUSTERSEARCHER_H
#define LUX_DIST_CLUSTERSEARCHER_H

#include "common.h"
#include "protocol.h"
#include "result.h"
#include "lux/search.h"
#include "server-config.pb.h"
#include <string>
#include <vector>
#include <deque>
#include <event.h>

namespace Lux {
namespace Dist {

  class ClusterSearcher;

  typedef enum {
    INITIALIZED,
    CONNECTING,
    READING,
    DONE,
    ERROR
  } status_t;

  typedef struct {
    response_header_t *h;
    uint32_t h_received;
    char *b;
    uint32_t b_received;
    uint32_t b_length;
  } response_t;

  typedef struct {
    response_t *resp;
    status_t status;
  } client_t;

  typedef struct {
    ClusterSearcher *ins;
    server_inf_t *serv;
    client_t *cli;
    command_t command;
    struct event e;
    // [TODO] add type class for polymorphismic send request
  } arg_t;

  typedef std::vector<server_inf_t> servers_inf;
  typedef servers_inf::iterator servers_inf_itr;
  typedef std::vector< std::deque<doc_id_t> > DocLocations; // document locations

  class ClusterSearcher {
  public:
    ClusterSearcher(Lux::Config::Cluster *cluster_config);
    ~ClusterSearcher(void);

    ResultSet search(const char *query, Condition &cond);
    ResultSet search(std::string query, Condition &cond);

    MergeResultSet search_servers_and_merge(const char *query,
                                            Condition &cond);
    MergeResultSet search_servers_and_merge(const std::string &query,
                                            Condition &cond);
    TmpResultSet get_docs_from_servers(MergeResultSet &mrs);

  private:
    str_vec server_name_list_;
    server_inf_t *servers_;
    client_t *clients_;
    arg_t *args_;
    uint32_t num_servers_;
    std::string query_;
    Condition cond_;
    DocLocations dloc_;
    static DocumentSerializer doc_serializer_;
    struct event_base *ebase_;

    void release_all(void);
    void empty_clients(void);
    bool nonblocking_connect(uint32_t sid, command_t command);
    bool search_indices(void);
    bool search_docs(void);
    bool merge_results(MergeResultSet &mrs);
    bool collect_doc_by_server(MergeResultSet &mrs);
    static void run(int fd, short event, void *arg);
    void handler(int fd, short event, void *arg);
    bool send_request(server_inf_t *serv, command_t command);
    bool send_search_request(server_inf_t *serv);
    bool send_getdocs_request(server_inf_t *serv);
    int recv_search_response(int fd, response_t *resp);
    void set_nonblocking(int &socket);
    void init_sockaddr(struct sockaddr_in &addr, in_addr_t ip, uint16_t port);
    ResultSet get_resultset(MergeResultSet &mrs);
    TmpResultSet get_tmp_resultset(MergeResultSet &mrs);
  };

}
}

#endif
