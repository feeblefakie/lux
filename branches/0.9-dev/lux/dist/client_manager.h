#ifndef LUX_DIST_CLIENTMANAGER_H
#define LUX_DIST_CLIENTMANAGER_H

#include "common.h"
#include "protocol.h"
#include "message_queue.h"

namespace Lux {
namespace Dist {

  template<class T, class Message>
  class ClientManager {
  public:
    ClientManager(std::vector<server_inf_t> servers)
    : servers_(servers), idx_(0)
    {}

    ~ClientManager()
    {}

    bool start_clients(void)
    {
      queues_ = new MessageQueue<Message>[servers_.size()];
      for (int i = 0; i < servers_.size(); ++i) {
        servers_[i].id = i;
        if (!create_thread(&servers_[i])) {
          error_log("create_thread() failed");
          return false;
        }
      }
      return true;
    }

    bool finish_clients(void)
    {
      std::cout << "finishing servers ..." << std::endl;
      for (int i = 0; i < threads_.size(); ++i) {
        threads_[i]->is_terminating = true;
      }

      for (int i = 0; i < threads_.size(); ++i) {
        if (pthread_join(threads_[i]->tid, NULL) != 0) {
          perror("pthread_join");
        }
        delete threads_[i];
      }
      delete [] queues_;
      return true;
    }

    bool enqueue_message(Message &message)
    {
      std::cout << "enqueued: " << (int) idx_ << std::endl;
      // put message into the queue 
      queues_[idx_].enqueue_message(message);
      // round robin
      if (++idx_ == servers_.size()) {
        idx_ = 0;
      }
    }

  private:
    uint32_t idx_;
    std::vector< client_thread_inf_t * > threads_;
    std::vector< server_inf_t > servers_;
    MessageQueue<Message> *queues_;

    bool create_thread(server_inf_t *server)
    {
      client_thread_inf_t *thread_inf = new client_thread_inf_t;
      thread_inf->server = server;
      thread_inf->is_terminating = false;
      thread_inf->queue = (void *) &queues_[server->id];
      if (pthread_create(&(thread_inf->tid), NULL,
                         launch, (void *) thread_inf) != 0) {
        perror("pthread_create");
        return false;
      }
      threads_.push_back(thread_inf);
      return true;
    }

    static void *launch(void *p)
    {
      T ins(p);
      ins.process();
      return NULL;
    }
  };

}
}

#endif
