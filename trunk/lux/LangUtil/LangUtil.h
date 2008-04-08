#ifndef LUX_LANGUTIL_H
#define LUX_LANGUTIL_H

#include <string>

namespace Lux {

    class LangUtil {
        
    public:
        virtual ~LangUtil() {};
        virtual int get_length_of(const unsigned char *str) const = 0;
        virtual int get_length_of(const std::string &str) const = 0;
    };
}

#endif
