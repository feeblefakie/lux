#ifndef LUX_NORMALIZER_H
#define LUX_NORMALIZER_H

#include <string>
#include <cctype>

namespace Lux {

  class Normalizer {

  public:
    static void process(std::string &org)
    {
      // temporary
      std::transform(org.begin(), org.end(), org.begin(), (int (*)(int)) std::tolower);       

      // [TODO] must do Japanese full-width and half-width kana normalization 
    }
  };
}

#endif
