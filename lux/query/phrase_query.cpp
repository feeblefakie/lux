#include "phrase_query.h"

namespace Lux {

  PhraseQuery *PhraseQuery::create(std::string phrase, std::string index)
  {
    PhraseQuery *phrase_query = new PhraseQuery(phrase, index);
    return phrase_query;
  }

  IndexResultSet PhraseQuery::search(SearchIndex *si)
  {
    return si->search_phrase(this);
  }

}
