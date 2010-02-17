#ifndef LUX_QUERY_TERMQUERY_H
#define LUX_QUERY_TERMQUERY_H

#include "query.h"
#include "lux/lexer/term.h"
#include "lux/index/search_index.h"
#include <string>

namespace Lux {

  class SearchIndex;

  class TermQuery : public Query {
  public:
    static TermQuery *create(Term term, std::string index = "default");
    virtual ~TermQuery(void) {};
    virtual IndexResultSet search(SearchIndex *si);
    Term get_term(void);
    std::string get_index(void);
    virtual std::string to_str(void);

  private:
    Term term_;
    std::string index_; // what index to look

    TermQuery(Term term, std::string index = "default")
    : term_(term), index_(index)
    {}
  };

}

#endif
