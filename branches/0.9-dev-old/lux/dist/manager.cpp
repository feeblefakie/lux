#include "manager.h"
#include "protocol.h"
#include "searcher.h"
#include "lux/search.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace Lux {
namespace Dist {

  Manager::Manager(std::string service, int sport, int iport)
  : service_(service),
    sport_(sport),
    iport_(iport),
    default_num_searchers_(3),
    max_num_seachers_(30),
    is_terminating_(false),
    dd_("tmp")
  {
    Lux::DocumentDefinition dd("blog");
    static Lux::FieldDefinition title_fd("title");
    title_fd.set_indexed(true);
    title_fd.set_displayed(true);
    dd.add(title_fd);
    static Lux::FieldDefinition content_fd("content");
    content_fd.set_indexed(true);
    content_fd.set_displayed(true);
    dd.add(content_fd);
    static Lux::FieldDefinition docnum_fd("docnum");
    //docnum_fd.set_attr_indexed(true, Lux::ATTR_INT, sizeof(int));
    docnum_fd.set_attr_indexed(true, Lux::ATTR_STR, 10);
    dd.add(docnum_fd);
    
    dd_ = dd;
  }

  Manager::~Manager(void)
  {}

  // [TODO] listen_sockets ? init_sockets
  bool Manager::listen(void)
  {
    if ((ssock_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      error_log("socket() failed.");
      return false;
    }
    int on = 1;
    setsockopt(ssock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    struct sockaddr_in saddr;
    memset((char *) &saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(sport_);

    if (bind(ssock_, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
      error_log("bind() failed.");
      return false;
    }

    // [TODO] set the same number as max_num_seachers_ ?
    ::listen(ssock_, 20); // temporary hardcoding

    // [TODO] create a socket for index

    return true;
  }

  bool Manager::start_servers(void)
  {
    stats_ = new global_thread_stats_t;
    stats_->num_searchers = 0;
    stats_->num_active_searchers = 0;
    stats_->num_processed = 0;
    stats_->mutex = new pthread_mutex_t;
    pthread_mutex_init(stats_->mutex, NULL);
    stats_->cond = new pthread_cond_t;
    pthread_cond_init(stats_->cond, NULL);

    Lux::Engine engine(dd_);
    if (!engine.open(service_.c_str(), Lux::DB_RDONLY)) {
      error_log("engine.open() failed");
      return false;
    }
    Lux::Searcher s(engine);

    for (int i = 0; i < default_num_searchers_; ++i) {
      if (!create_searcher(s)) {
        error_log("create_searcher() failed");
      }
    }

    // manager loop
    pthread_mutex_lock(stats_->mutex);
    while (!is_terminating_) {
      std::cout << "waiting ..." << std::endl;
      pthread_cond_wait(stats_->cond, stats_->mutex);
      std::cout << "[manager] num_active_searchers: " << stats_->num_active_searchers << "\n"
                << "[manager] num_searchers: " << stats_->num_searchers << std::endl;
      if (stats_->num_active_searchers >= stats_->num_searchers) {
        std::cout << "better to add more threads" << std::endl;
        if (stats_->num_searchers < max_num_seachers_) {
          if (!create_searcher(s)) {
            error_log("create_searcher() failed");
          }
        }
      } else {
        uint16_t dec_threshold = default_num_searchers_
                                + (stats_->num_searchers - default_num_searchers_) / 2;
        if (stats_->num_active_searchers < dec_threshold) {
          if (stats_->num_searchers > default_num_searchers_) {
            if (!send_internal_close_request(TYPE_SEARCHER)) {
              error_log("send_internal_close_request() failed.");
            }
          } 
        }
      }
    }
    pthread_mutex_unlock(stats_->mutex);

    finish_servers();

    return true;
  }

  bool Manager::finish_servers(void)
  {
    // send internal dummy connection
    pthread_mutex_lock(stats_->mutex);
    for (int i = 0; i < stats_->num_searchers; ++i) {
      if (!send_internal_close_request(TYPE_SEARCHER)) {
        error_log("send_internal_close_request() failed.");
      }
    }
    pthread_mutex_unlock(stats_->mutex);

    int retry = 10;
    while (--retry) {
      pthread_mutex_lock(stats_->mutex);
      if (stats_->num_searchers == 0) { break; }
      pthread_mutex_unlock(stats_->mutex);
      sleep(3);
    }
    close(ssock_);
    return true;
  }

  bool Manager::create_searcher(Lux::Searcher &s)
  {
    thread_inf_t *thread_inf = new thread_inf_t;
    thread_inf->sock = ssock_;
    thread_inf->stats = stats_;
    thread_inf->s = &s;
    pthread_create(&thread_inf->tid, NULL, launch_searcher, (void *) thread_inf);
    if (pthread_detach(thread_inf->tid) != 0) {
      error_log("pthread_detach failed");
      return false;
    }
    return true;
  }

  bool Manager::send_internal_close_request(service_type_t type)
  {
    icr_arg_t *arg = new icr_arg_t;
    arg->type = type;
    arg->ins = this;
    pthread_create(&arg->tid, NULL, Manager::run_icr, (void *) arg);
    if (pthread_detach(arg->tid) != 0) {
      error_log("pthread_detach failed");
      return false;
    }
    return true;
  }

  /* this is called by another thread */
  void *Manager::run_icr(void *arg)
  {
    if (!(((icr_arg_t *) arg)->ins)->send_quit(((icr_arg_t *) arg)->type)) {
      error_log("send_quit failed."); 
    }
    return NULL;
  }

  bool Manager::send_quit(service_type_t type)
  {
    struct hostent *he;
    if ((he = gethostbyname("localhost")) == NULL) {
      error_log("gethostbyname failed");
      return false;
    }
    uint32_t dest_ip;
    memcpy((char *) &dest_ip, (char *) he->h_addr, he->h_length);

    int s;
    struct sockaddr_in server;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return false;
    }

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = dest_ip;
    server.sin_port = (type == TYPE_SEARCHER) ? 
                        htons(sport_) : htons(iport_);

    if (lut_connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
      perror("connect");
      return false;
    }
    request_header_t header = {0, 0, COM_QUIT};
    if (lut_send(s, &header, sizeof(header), 0) < 0) {
      perror("send");
      return false;
    }

    response_header_t res;
    if (lut_recv(s, (char *) &res, sizeof(res), 0) <= 0) {
      return false;
    }
    close(s);

    if (res.status != STATUS_OK) {
      return false;
    }
    return true;
  }

}
}
