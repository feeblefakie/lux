#include "handler.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

#undef USE_ZLIB

namespace Lux {
namespace Dist {

  Handler::Handler(void *p)
  : inf_((thread_inf_t *) p), 
    is_terminating_(false)
  {
    stats_ = inf_->stats;
  }

  Handler::~Handler(void)
  {
    delete inf_;
  }

  void Handler::process(void)
  {
    inc_thread_();

    while (1) {
      struct sockaddr_in caddr;
      socklen_t caddr_len = sizeof(caddr);
      if ((con_ = lut_accept(inf_->sock,
                        (struct sockaddr *) &caddr, &caddr_len)) < 0) {
        error_log("lut_accept failed.");
        continue;
      }
      int on = 1;
      setsockopt(con_, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

      pthread_mutex_lock(stats_->mutex);
      ++(stats_->num_active_threads);
      if (stats_->num_active_threads >= stats_->num_threads) {
        // it's urgent. need to add active.
        std::cout << "sending signal" << std::endl;
        pthread_cond_signal(stats_->cond);
      }
      pthread_mutex_unlock(stats_->mutex);

      bool is_client_up = true;
      do {
        request_header_t header;
        char *body = NULL;
        uint32_t body_len;
        bool res = recv_packets(header, &body, &body_len);
        // [TODO] sending back error packets ?

        if (res) { // only if recv_packets succeeds
          if (!exec(header, body, body_len)) {
            error_log("exec() failed.");
          }
        } else if (res == 0) {
          is_client_up = false;
        }

        if (body != NULL) {
          delete [] body;
        }
      } while (is_persistent_ && is_client_up);

      if (shutdown(con_, SHUT_WR) < 0) {
        perror("shutdown");
      } 
      int tmp_buf;
      while (1) {
        if (lut_recv(con_, (char *) &tmp_buf, sizeof(int), 0) == 0) {
          break;
        } 
      } 
      close(con_);

      pthread_mutex_lock(stats_->mutex);
      --(stats_->num_active_threads);
      ++(stats_->num_processed); // set 0 automatically when overflowed
      if ((stats_->num_processed % 20) == 0) {
        // sending signals sometimes to check spare threads 
        pthread_cond_signal(stats_->cond);
      }
      pthread_mutex_unlock(stats_->mutex);

      if (is_terminating_) { break; }
    }

    dec_thread_();
  }

  int Handler::recv_packets(request_header_t &header,
                                char **body, uint32_t *len)
  {
    ssize_t res = lut_recv(con_, (char *) &header, sizeof(header), 0);
    if (res <= 0) {
      error_log("lut_recv() failed");
      return res;
    }

    *len = header.length;
    if (*len != 0) {
      *body = new char[*len];
      if (lut_recv(con_, *body, *len, 0) <= 0) {
        error_log("lut_recv() failed");
        return false;
      }
    }
    // -1: recv error
    //  0: client down
    //  1: success
    return 1;
  }

  bool Handler::send_packets(char *data, uint32_t size,
                             resp_status_t status)
  {
    response_header_t res = {size, 0, status};

    char *actual_data = data; 
    uint32_t actual_size = size;
#ifdef USE_ZLIB
    double t1 = gettimeofday_sec();
    char *comp = compress(data, size, &actual_size);
    if (comp != NULL) {
      res.length = size; // original size
      res.comp_length = actual_size;
      actual_data = comp;
    } else {
      actual_size = size;
    }
    double t2 = gettimeofday_sec();
    std::cout << "compress time: " << t2 - t1 << std::endl;
#endif

    if (actual_size > (1 << PACKET_BODY_MAX_BITS)) {
      error_log("sending data is too long. failing ...");
      return false;
    }

    struct iovec v[2];
    v[0].iov_base = &res;
    v[0].iov_len = sizeof(res);
    v[1].iov_base = actual_data;
    v[1].iov_len = actual_size;

    if (lut_writev(con_, v, 2) < 0) {
      error_log("lut_writev() failed");
      return false;
    }

    return true;
  }

  bool Handler::send_ok(void)
  {
    response_header_t res = {0, 0, STATUS_OK};

    if (lut_send(con_, &res, sizeof(res), 0) < 0) {
      error_log("send() failed");
      return false;
    }
    return true;
  }

  bool Handler::send_error(err_code_t err_code)
  {
    uint8_t code = err_code;
    response_header_t res = {sizeof(code), 0, STATUS_ERR};

    if (lut_send(con_, &res, sizeof(res), 0) < 0) {
      error_log("send() failed");
      return false;
    }
  
    if (lut_send(con_, &code, sizeof(code), 0) < 0) {
      error_log("send() failed");
      return false;
    }
    return true;
  }

  bool Handler::exec_quit(void)
  {
    std::cout << "QUIT RECEIVED" << std::endl;
    response_header_t res = {0, 0, STATUS_OK};
    if (lut_send(con_, &res, sizeof(res), 0) < 0) {
      error_log("send() failed");
      return false;
    }
    return true;
  }

  void Handler::inc_thread_(void)
  {
    pthread_mutex_lock(stats_->mutex);
    ++(stats_->num_threads);
    pthread_mutex_unlock(stats_->mutex);
  }

  void Handler::dec_thread_(void)
  {
    pthread_mutex_lock(stats_->mutex);
    --(stats_->num_threads);
    pthread_mutex_unlock(stats_->mutex);
  }

}
}
