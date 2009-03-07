#ifndef LUX_LANG_UTF8UTIL_H
#define LUX_LANG_UTF8UTIL_H

#include "lang_util.h"

namespace Lux {

  class UTF8Util : public LangUtil {
  public:
    UTF8Util(void) {};
    virtual ~UTF8Util() {};
    int get_length_of(const unsigned char *str) const;
    int get_length_of(const std::string &str) const;

  };
}

#endif
