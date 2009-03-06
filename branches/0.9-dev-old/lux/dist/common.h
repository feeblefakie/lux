#ifndef LUX_DIST_COMMON_H
#define LUX_DIST_COMMON_H

#include "lux/lux.h"
#include "util.h"
#include <pthread.h>
#include <string>
#include <vector>

// temporary. should be moved
typedef int sock_t;
typedef uint32_t server_id_t;

typedef std::vector<std::string> str_vec;
typedef str_vec::iterator str_vec_itr;

namespace Lux {
namespace Dist {

  // it's shared in all threads,
  // so it must be locked before update
  typedef struct {
    uint16_t num_threads;
    uint16_t num_active_threads;
    uint32_t num_processed;
    pthread_mutex_t *mutex;
    pthread_cond_t *cond;
  } global_thread_stats_t;

  typedef struct {
    pthread_t tid; // thread id
    sock_t sock;
    global_thread_stats_t *stats;
    void *supp_arg; // supplementary thread argument
  } thread_inf_t;

  typedef struct {
    in_addr_t ip;
    sock_t socket;
    uint16_t port;
    uint32_t id;
  } server_inf_t;

  typedef struct {
    uint16_t num_threads;
    pthread_mutex_t *mutex;
  } global_client_thread_inf_t;

  typedef struct {
    pthread_t tid; // thread id
    server_inf_t *server;
    void *queue;
    bool is_terminating;
  } client_thread_inf_t;

}
}

#endif
