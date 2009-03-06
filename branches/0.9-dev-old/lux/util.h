#ifndef LUX_UTIL_H
#define LUX_UTIL_H

//#define _XOPEN_SOURCE 50

#include "luxconfig.h"
#include "types.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <unistd.h>

#ifdef USE_ZLIB
#include <zlib.h>
#endif

#define MIN_COMPRESS_LENGTH 50

#ifdef DEBUG
#define error_log(msg) \
  std::cerr << "[error] " << msg \
            << " in " << __FILE__ << ":" << __LINE__ \
            << std::endl; 
#else
#define error_log(msg)
#endif

// for verbose info logging
#ifdef DEBUG_VINFO
#define vinfo_log(msg) \
  std::cerr << "[info] " << msg \
            << std::endl; 
#else
#define vinfo_log(msg)
#endif

namespace Lux {

  static inline void _mkdir(const char *str)
  {
    std::string str_(str); 
    std::size_t n = 0;
    while (1) {
      n = str_.find_first_of('/', n+1);
      if (n == std::string::npos) {
        break;  
      }
      std::string dir = str_.substr(0, n);
      // [TODO] error handling
      ::mkdir(dir.c_str(), 0755);
    }
  }

  static inline int _open(const char *pathname, int flags, mode_t mode)
  {
    int oflags = O_RDONLY;

    if (flags & DB_RDWR) {
      oflags |= O_RDWR;
    }
    if (flags & DB_CREAT) {
      oflags |= O_CREAT | O_RDWR;
      _mkdir(pathname);
    }
    if (flags & DB_TRUNC) {
      oflags |= O_TRUNC;
    }
    return ::open(pathname, oflags, mode);
  }
  
  static inline ssize_t _read(int fd, void *buf, size_t count)
  {
    char *p = reinterpret_cast<char *>(buf);
    const char * const end_p = p + count;

    while (p < end_p) {
      const int num_bytes = read(fd, p, end_p - p);
      if (num_bytes < 0) {
        if (errno == EINTR) {
          continue;
        }
        perror("read failed");
        break;
      }
      p += num_bytes;
    }

    if (p != end_p) {
      return -1;
    }
    return count;
  }

  static inline ssize_t _write(int fd, const void *buf, size_t count)
  {
    const char *p = reinterpret_cast<const char *>(buf);
    const char * const end_p = p + count;

    while (p < end_p) {
      const int num_bytes = write(fd, p, end_p - p);
      if (num_bytes < 0) {
        if (errno == EINTR) continue;
        perror("write failed");
        break;
      }
      p += num_bytes;
    }

    if (p != end_p) {
      return -1;
    }
    return count;
  }

  static inline bool _pwrite(int fd, const void *buf, size_t nbyte, off_t offset)
  {
    if (pwrite(fd, buf, nbyte, offset) < 0) {
      perror("pwrite failed");
      return false;
    }
    /*
    if (lseek(fd, offset, SEEK_SET) < 0) {
      return false;
    }
    if (_write(fd, buf, nbyte) < 0) {
      return false; 
    }
    */
    return true;
  }

  static inline bool _pread(int fd, void *buf, size_t nbyte, off_t offset)
  {
    if (pread(fd, buf, nbyte, offset) < 0) {
      perror("pread failed");
      return false;
    }
    /*
    if (lseek(fd, offset, SEEK_SET) < 0) {
      return false;
    }
    if (_read(fd, buf, nbyte) < 0) {
      return false; 
    } 
    */
    return true;
  }

  static inline void *_mmap(int fd, size_t size, int flags)
  {
    int prot = PROT_READ;
    if (flags & DB_RDWR || flags & DB_CREAT) {
      prot |= PROT_WRITE;
    }

    void *p = mmap(0, size, prot, MAP_SHARED, fd, 0);  
    if (p == MAP_FAILED) {
      perror("mmap failed.");
      return NULL;
    }
    return p;
  }

#ifdef USE_ZLIB
  static inline
  char *_compress(char *src, uint32_t slen, uint32_t *dlen)
  {
    *dlen = slen * 120 / 100 + 12;
    char *dest = new char[*dlen];
    int res = ::compress((Bytef *) dest, (uLongf *) dlen,
                         (const Bytef *) src, (uLong) slen);
    if (res != Z_OK) { 
      delete [] dest;
      if (res == Z_MEM_ERROR) {
        error_log("out of memory");
      } else if (res == Z_BUF_ERROR) {
        error_log("no enough buffer");
      }
      return NULL;
    }
    if (*dlen > slen) {
      *dlen = 0;
      delete [] dest;
      return NULL; // no compression
    }
    return dest;
  }

  static inline
  char *compress(char *src, uint32_t slen, uint32_t *dlen)
  {
    if (slen < MIN_COMPRESS_LENGTH) {
      *dlen = 0;
      return NULL;
    }
    return _compress(src, slen, dlen);
  }

  static inline
  char *uncompress(char *src, uint32_t slen, uint32_t *dlen)
  {
    char *dest = new char[*dlen+1]; // must be smaller than slen
    memset(dest, 0, *dlen+1);
    int res = ::uncompress((Bytef *) dest, (uLongf *) dlen,
                           (const Bytef *) src, (uLong) slen);
    if (res != Z_OK) { 
      delete [] dest;
      if (res == Z_MEM_ERROR) {
        error_log("out of memory");
      } else if (res == Z_BUF_ERROR) {
        error_log("no enough buffer");
      }
      return NULL;
    }
    return dest;
  }
#endif

}

#endif
