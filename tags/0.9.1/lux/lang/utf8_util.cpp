#include "utf8_util.h"
#include <cstring>

namespace Lux {

  int UTF8Util::get_length_of(const unsigned char *str) const
  {
    size_t str_len = std::strlen((char *) str);
    int ustr_len = 0;

    for (int i = 0; i < str_len; i++) {
      if (str[i] <= 0x7f || (str[i] >= 0xc0 && str[i] <= 0xfd)) {
        ustr_len++;
      }
    }
    return ustr_len;
  }

  int UTF8Util::get_length_of(const std::string &str) const
  {
    return get_length_of(reinterpret_cast<const unsigned char *>(str.c_str()));
  }

}
