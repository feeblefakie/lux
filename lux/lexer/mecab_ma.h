#ifndef LUX_LEXER_MECABMA_H
#define LUX_LEXER_MECABMA_H

#include "term.h"
#include "lux/scoped_ptr.h"
#include <vector>
#include <mecab.h>

namespace Lux {

  typedef std::vector<Term> TokenContainer;
  typedef TokenContainer::iterator TCIterator;

  class LangUtil;

  class MecabMA {

  public:
    MecabMA();
    bool tokenize(const char *str);
    bool tokenize(const std::string &str);
    Term get_next_token();
    bool has_next();
    void init_iter();
    void clear();
  
  /* destructor must be called by a derived class */
  protected:
    ~MecabMA();

  private:
    TokenContainer tokens_;
    TCIterator tokens_iterator_;
    scoped_ptr<MeCab::Tagger> tagger_;
    scoped_ptr<Lux::LangUtil> lang_util_;
  };
}

#endif
