#ifndef LUX_DIST_MANAGER_H
#define LUX_DIST_MANAGER_H

#include "common.h"
#include "lux/Document/DocumentDefinition.h"
#include "lux/search.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>

namespace Lux {
namespace Dist {

  class Manager;

  typedef enum {
    TYPE_SEARCHER,
    TYPE_INDEXER
  } service_type_t;

  typedef struct {
    pthread_t tid; // thread id
    service_type_t type;
    Manager *ins;
  } icr_arg_t; // icr: internal close request

  class Manager {
  public:
    Manager(std::string service, int sport, int iport);
    ~Manager();
    bool listen(void);
    bool start_servers(void);
    bool finish_servers(void);
  private:
    std::string service_;
    int sport_;
    int iport_;
    sock_t ssock_; // server socket for search threads
    sock_t isock_; // server socket for indexing thread
    uint16_t default_num_searchers_;
    uint16_t max_num_seachers_;
    bool is_terminating_;
    global_thread_stats_t *stats_;
    Lux::DocumentDefinition dd_;

    bool create_searcher(Lux::Searcher &s);
    bool send_internal_close_request(service_type_t type);
    static void *run_icr(void *arg);
    bool send_quit(service_type_t type);
  };

}
}

#endif
