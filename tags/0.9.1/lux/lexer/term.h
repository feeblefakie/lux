#ifndef LUX_LEXER_TERM_H
#define LUX_LEXER_TERM_H

#include <string>

namespace Lux {

  class Term {
  public:
    Term(std::string text = "", unsigned int length = 0, unsigned int offset = 0);
    Term operator+(Term &term);
    Term &operator+=(Term &term);

    std::string text_;
    unsigned int length_;
    unsigned int offset_;
  };
}

#endif
