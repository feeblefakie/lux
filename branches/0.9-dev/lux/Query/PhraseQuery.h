#ifndef LUX_PHRASEQUERY_H
#define LUX_PHRASEQUERY_H

#include "Query.h"
#include "lux/Storage/SearchIndex.h"
#include <string>
#include <vector>
#include <iostream>

namespace Lux {

  class PhraseQuery : public Query {

  public:
    static PhraseQuery *create(std::string phrase, std::string index = "default")
    {
      PhraseQuery *phrase_query = new PhraseQuery(phrase, index);
      return phrase_query;
    }

    virtual ~PhraseQuery()
    {}

    virtual IndexResultSet search(SearchIndex *si)
    {
      return si->search_phrase(this);
    }

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
