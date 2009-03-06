#ifndef LUX_TERMQUERY_H
#define LUX_TERMQUERY_H

#include "Query.h"
#include "lux/Lexer/Term.h"
#include "lux/Storage/SearchIndex.h"
#include <string>
#include <iostream>

namespace Lux {

  class TermQuery : public Query {

  public:
    static TermQuery *create(Term term, std::string index = "default")
    {
      TermQuery *term_query = new TermQuery(term, index);
      return term_query;
    } 

    virtual ~TermQuery()
    {}

    virtual IndexResultSet search(SearchIndex *si)
    {
      si->search_term(this);
    }

    Term get_term(void)
    {
      return term_;
    }

    std::string get_index(void)
    {
      return index_;
    }

    virtual std::string to_str(void)
    {
      return term_.text_;
    }

  private:
    Term term_;
    std::string index_; // what index to look

    TermQuery(Term term, std::string index = "default")
    : term_(term), index_(index)
    {}
  };

}

#endif
