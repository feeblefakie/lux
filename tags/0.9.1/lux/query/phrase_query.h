#ifndef LUX_QUERY_PHRASEQUERY_H
#define LUX_QUERY_PHRASEQUERY_H

#include "query.h"
#include "lux/index/search_index.h"
#include <string>
#include <vector>
#include <iostream>

namespace Lux {

  class PhraseQuery : public Query {
  public:
    static PhraseQuery *create(std::string phrase, std::string index = "default");
    virtual ~PhraseQuery(void) {}
    virtual IndexResultSet search(SearchIndex *si);
    std::string get_phrase(void) { return phrase_; }
    std::string get_index(void) { return index_; }
    virtual std::string to_str(void) { return phrase_; }

  private:
    std::string phrase_;
    std::string index_; // what index to look

    PhraseQuery(std::string phrase, std::string index = "default")
    : phrase_(phrase), index_(index)
    {}

  };

}

#endif
