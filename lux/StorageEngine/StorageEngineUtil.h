#ifndef LUX_STORAGEENGINEUTIL_H
#define LUX_STORAGEENGINEUTIL_H

#include <sys/stat.h>
#include <sys/types.h>
#include <string>

namespace Lux {
 
  /**
   * make directory recursively for unix-like filesystem
   */
  inline void
  mkdir(const char *str)
  {
    std::string str_(str); 
    int n = -1;
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
        
}

#endif
