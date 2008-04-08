#ifndef LUX_INDEXRESULT_H
#define LUX_INDEXRESULT_H

#include "lux/lux.h"
#include <vector>

namespace Lux {

  struct IndexResult {
    IndexResult(doc_id_t doc_id_, unsigned int score_)
    : doc_id(doc_id_), score(score_)
    {}
    doc_id_t doc_id;
    union {
      unsigned int score;
      unsigned int attr;
    };
    // Document *d;
    // any attribute to be sorted
  };

  typedef std::vector<IndexResult> IndexResultSet;
  typedef IndexResultSet::iterator IRSIterator;

}

#endif
