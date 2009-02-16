#ifndef LUX_LEXER_H
#define LUX_LEXER_H

#include "MecabMA.h"

namespace Lux {

  template <class LexingPolicy>
  class Lexer : public LexingPolicy {

    /*
     *  if you want to add codes before or after the method
     *  (something like degugging codes and so on),
     *  you can easily create a function having the same name.
     */
  };
}

typedef Lux::Lexer<Lux::MecabMA> LuxLexer;

#endif
