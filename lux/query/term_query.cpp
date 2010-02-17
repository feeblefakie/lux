#include "term_query.h"

namespace Lux {

  TermQuery *TermQuery::create(Term term, std::string index)
  {
    TermQuery *term_query = new TermQuery(term, index);
    return term_query;
  } 

  IndexResultSet TermQuery::search(SearchIndex *si)
  {
    si->search_term(this);
  }

  Term TermQuery::get_term(void)
  {
    return term_;
  }

  std::string TermQuery::get_index(void)
  {
    return index_;
  }

  std::string TermQuery::to_str(void)
  {
    return term_.text_;
  }

}
