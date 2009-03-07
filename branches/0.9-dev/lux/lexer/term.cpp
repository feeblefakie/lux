#include "term.h"

namespace Lux {

  Term::Term(std::string text, unsigned int length, unsigned int offset)
  : text_(text), length_(length), offset_(offset)
  {}

  Term Term::operator+(Term &term)
  {
    std::string text = text_ + term.text_;
    unsigned int length = length_ + term.length_;
    unsigned int offset = offset_;
    return Term(text, length, offset);
  }

  Term &Term::operator+=(Term &term)
  {
    text_ += term.text_;
    length_ += term.length_;
    // no change in offset_
    return *this;
  }

}
