/**
 * This file is temporary. should be merged into lux/util.h
 */
#ifndef LUX_SERVER_UTIL_H
#define LUX_SERVER_UTIL_H

#include "lux/lux.h"
#include "common.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <netdb.h>

#include <time.h>
#include <sys/time.h>

// System call wrapper used in gauche.
// (macro names are changed.)
// thanks to shirok and kzk.
// http://kzk9.net/column/write.html
#define SAFE_SYSCALL3(result, expr, check) \
  do { \
    (result) = (expr); \
    if ((check) && errno == EINTR) { \
      errno = 0; \
    } else { \
      break; \
    } \
  } while (1)

#define SAFE_SYSCALL(result, expr) \
  SAFE_SYSCALL3(result, expr, (result < 0))

namespace Lux {

  static inline
  double gettimeofday_sec()
  {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
  }

  static inline
  int lut_connect(int s, const struct sockaddr *serv_addr, socklen_t addrlen)
  {
    int ret;
    SAFE_SYSCALL(ret, connect(s, serv_addr, addrlen));
    return ret;
  }

  static inline
  int lut_accept(int s, struct sockaddr *addr, socklen_t *addrlen)
  {
    int ret;
    SAFE_SYSCALL(ret, accept(s, addr, addrlen));
    return ret;
  }

  static inline
  ssize_t lut_recv(int s, void *buf, size_t len, int flags)
  {
    char *p = reinterpret_cast<char *>(buf);
    const char * const end_p = p + len;

    while (p < end_p) {
      ssize_t num_bytes;
      SAFE_SYSCALL(num_bytes, recv(s, p, end_p - p, flags));
      if (num_bytes < 0) {
        return num_bytes;
      } else if (num_bytes == 0) {
        return num_bytes;
      }
      p += num_bytes;
    }

    if (p != end_p) {
      return -1;
    }
    return len;
  }

  static inline
  ssize_t lut_send(int s, const void *buf, size_t len, int flags)
  {
    const char *p = reinterpret_cast<const char *>(buf);
    const char * const end_p = p + len;

    while (p < end_p) {
      ssize_t num_bytes;
      SAFE_SYSCALL(num_bytes, send(s, p, end_p - p, flags));
      if (num_bytes < 0) {
        perror("send failed");
        break;
      }
      p += num_bytes;
    }

    if (p != end_p) {
      return -1;
    }
    return len;
  }

  static inline
  ssize_t lut_writev(int fd, struct iovec *vector, int count)
  {
    size_t total = 0;
    for (int i = 0; i < count; ++i) {
      total += vector[i].iov_len;
    }

    size_t remain = total;
    while (1) {
      ssize_t num_bytes;
      SAFE_SYSCALL(num_bytes, writev(fd, vector, count));
      if (num_bytes < 0) {
        perror("writev failed");
        break;
      }
      remain -= num_bytes;
      if (!remain) { break; }

      for (int i = 0; i < count; ++i) {
        if (num_bytes >= vector[i].iov_len) {
          num_bytes -= vector[i].iov_len;
        } else {
          vector[i].iov_len -= num_bytes;
          char *p = (char *) vector[i].iov_base;
          p += num_bytes;
          vector[i].iov_base = p;
          int j = 0;
          for (; i < count && j < count; ++i, ++j) {
            vector[j] = vector[i];
          }
          count = j;
        }
      }
    }
    if (remain > 0) {
      return -1;
    }
    return total;
  }

  static inline
  bool sock_connect(int &s, in_addr_t ip, uint16_t port)
  {
    struct sockaddr_in server;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      return false;
    }
    int on = 1;
    setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = ip;
    server.sin_port = htons(port);

    if (lut_connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
      perror("connect");
      return false;
    }
    return true;
  }

  static inline
  in_addr_t get_ip_by_str(const char *str)
  {
    in_addr_t ip;
    if ((ip = inet_addr(str)) == INADDR_NONE) {
      struct hostent *he;
      if ((he = gethostbyname(str)) == NULL) {
        return 0;
      }   
      memcpy((char *) &ip, (char *) he->h_addr, he->h_length);
    }
    return ip;
  }
}

#endif
