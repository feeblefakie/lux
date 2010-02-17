#ifndef LUX_DIST_SEARCHER_H
#define LUX_DIST_SEARCHER_H

#include "common.h"
#include "protocol.h"
#include "server_config_helper.h"
#include "lux/search.h"
#include "lux/scoped_ptr.h"
#include "lux/document/document_serializer.h"
#include <vector>

namespace Lux {
namespace Dist {

  typedef uint32_t dispatcher_id;
  typedef struct {
    in_addr_t ip;
    int port;
  } dispatcher_inf;

  typedef std::vector<dispatcher_inf> dispatchers_inf;

  class Searcher {
  public:
    Searcher(std::string service);
    ~Searcher(void);
    ResultSet search(const char *query, Condition &cond);
    ResultSet search(std::string query, Condition &cond);

  private:
    //dispatchers_inf dispatchers_;
    scoped_ptr<ServerConfigHelper> config_helper_;
    Lux::Config::Dispatchers dispatchers_;
    static DocumentSerializer doc_serializer_;

    bool send_search_request(sock_t s,
                             std::string &query, Condition &cond);
    bool recv_search_request(sock_t s, ResultSet &rs);
    bool deserialize_rs(ResultSet &rs, char *body, uint32_t len);
  };

}
}

#endif
