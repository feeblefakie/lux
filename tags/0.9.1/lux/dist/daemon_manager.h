#ifndef LUX_DIST_DAEMONMANAGER_H
#define LUX_DIST_DAEMONMANAGER_H

#include "common.h"
#include "protocol.h"
#include "lux/util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <vector>

#define ARR_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

namespace Lux {
namespace Dist {

  typedef struct {
    pthread_t tid; // thread id
    void *ins;
  } at_arg_t; // at: admin thread

  int signals[] = {SIGINT, SIGQUIT, SIGTERM};

  template<class T>
  class DaemonManager {
  public:
    DaemonManager(int port, uint16_t default_num_threads,
            uint16_t max_num_threads)
    : port_(port),
      default_num_threads_(default_num_threads),
      max_num_threads_(max_num_threads),
      is_terminating_(false),
      supp_(NULL)
    {
      sigset_t ss;
      sigemptyset(&ss);
      sigfillset(&ss);
      sigprocmask(SIG_BLOCK, &ss, 0);
    }

    ~DaemonManager()
    {}

    bool listen(void)
    {
      if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error_log("socket() failed.");
        return false;
      }
      int on = 1;
      setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
      setsockopt(sock_, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

      struct sockaddr_in saddr;
      memset((char *) &saddr, 0, sizeof(saddr));
      saddr.sin_family = AF_INET;
      saddr.sin_addr.s_addr = htonl(INADDR_ANY);
      saddr.sin_port = htons(port_);

      if (bind(sock_, (struct sockaddr *) &saddr, sizeof(saddr)) < 0) {
        error_log("bind() failed.");
        return false;
      }

      ::listen(sock_, max_num_threads_); // temporary hardcoding

      return true;
    }

    void add_managee_arg(void *supp)
    {
      supp_ = supp;
    }

    bool start_servers(void)
    {
      stats_ = new global_thread_stats_t;
      stats_->num_threads = 0;
      stats_->num_active_threads = 0;
      stats_->num_processed = 0;
      stats_->mutex = new pthread_mutex_t;
      pthread_mutex_init(stats_->mutex, NULL);
      stats_->cond = new pthread_cond_t;
      pthread_cond_init(stats_->cond, NULL);

      for (int i = 0; i < default_num_threads_; ++i) {
        if (!create_thread()) {
          error_log("create_thread() failed");
        }
      }

      if (!create_signal_handler()) {
        return false;
      }

      // manager loop
      pthread_mutex_lock(stats_->mutex);
      while (!is_terminating_) {
        pthread_cond_wait(stats_->cond, stats_->mutex);
        if (is_terminating_) { break; }
        std::cout << "[manager] num_active_threads: " << stats_->num_active_threads << "\n"
                  << "[manager] num_threads: " << stats_->num_threads << std::endl;
        if (stats_->num_active_threads >= stats_->num_threads) {
          std::cout << "better to add more threads" << std::endl;
          if (stats_->num_threads < max_num_threads_) {
            if (!create_thread()) {
              error_log("create_thread() failed");
            }
          }
        } else {
          uint16_t dec_threshold = default_num_threads_
                                  + (stats_->num_threads - default_num_threads_) / 2;
          if (stats_->num_active_threads < dec_threshold) {
            if (stats_->num_threads > default_num_threads_) {
              if (!send_internal_close_request()) {
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

    bool finish_servers(void)
    {
      std::cout << "finishing servers ..." << std::endl;
      // send internal dummy connection
      pthread_mutex_lock(stats_->mutex);
      for (int i = 0; i < stats_->num_threads; ++i) {
        if (!send_internal_close_request()) {
          error_log("send_internal_close_request() failed.");
        }
      }
      pthread_mutex_unlock(stats_->mutex);

      int retry = 30;
      while (--retry) {
        sleep(1);
        pthread_mutex_lock(stats_->mutex);
        if (stats_->num_threads == 0) { break; }
        pthread_mutex_unlock(stats_->mutex);
      }
      close(sock_);

      delete stats_->mutex;
      delete stats_->cond;
      delete stats_;

      return true;
    }

  private:
    int port_;
    sock_t sock_;
    uint16_t default_num_threads_;
    uint16_t max_num_threads_;
    bool is_terminating_;
    global_thread_stats_t *stats_;
    void *supp_;

    bool create_thread(void)
    {
      thread_inf_t *thread_inf = new thread_inf_t;
      thread_inf->sock = sock_;
      thread_inf->stats = stats_;
      thread_inf->supp_arg = supp_;
      if (pthread_create(&thread_inf->tid, NULL,
                         launch, (void *) thread_inf) != 0) {
        perror("pthread_create");
        return false;
      }
      if (pthread_detach(thread_inf->tid) != 0) {
        error_log("pthread_detach failed");
        return false;
      }
      return true;
    }

    static void *launch(void *p)
    {
      T ins(p);
      ins.process();
      return NULL;
    }

    bool send_internal_close_request(void)
    {
      at_arg_t *arg = new at_arg_t;
      arg->ins = this;
      if (pthread_create(&arg->tid, NULL,
                         DaemonManager::run_icr, (void *) arg) != 0) {
        perror("pthread_create");
        return false;
      }
      if (pthread_detach(arg->tid) != 0) {
        error_log("pthread_detach failed");
        return false;
      }
      return true;
    }

    static void *run_icr(void *arg)
    {
      if (!((DaemonManager *) ((at_arg_t *) arg)->ins)->send_quit()) {
        error_log("send_quit failed."); 
      }
      delete (at_arg_t *) arg;
      return NULL;
    }

    bool send_quit(void)
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
      server.sin_port = htons(port_);

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

    bool create_signal_handler(void)
    {
      at_arg_t *arg = new at_arg_t;
      arg->ins = this;
      pthread_create(&arg->tid, NULL, DaemonManager::run_signal_handler, (void *) arg);
      if (pthread_detach(arg->tid) != 0) {
        error_log("pthread_detach failed");
        return false;
      }
      return true;
    }

    static void *run_signal_handler(void *arg)
    {
      if (!((DaemonManager *) ((at_arg_t *) arg)->ins)->handle_signals()) {
        error_log("send_quit failed."); 
      }
      delete (at_arg_t *) arg;
      return NULL;
    }

    bool handle_signals(void)
    {
      sigset_t ss;
      int sig;

      sigemptyset(&ss);
      for(uint32_t i = 0; i < ARR_SIZE(signals); ++i) {
        sigaddset(&ss, signals[i]);
      }
      pthread_sigmask(SIG_BLOCK, &ss, 0);

      while (!is_terminating_) {
        if (sigwait(&ss, &sig)) {
          error_log("signal error.");
          continue;
        }
        for (uint32_t i = 0; i < ARR_SIZE(signals); ++i) {
          if (signals[i] != sig) {
            continue; // ignored
          }
          std::cout << "signal received: " << sig << std::endl;
          is_terminating_ = true;
          pthread_mutex_lock(stats_->mutex);
          pthread_cond_signal(stats_->cond);
          pthread_mutex_unlock(stats_->mutex);
          break;
        }
      }
      return true;
    }
  };

}
}

#endif
