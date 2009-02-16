#ifndef LUX_INDEXRESULT_H
#define LUX_INDEXRESULT_H

#include "lux/lux.h"
#include <vector>

namespace Lux {

  struct IndexResult {
    IndexResult(doc_id_t doc_id_, score_t score_)
    : doc_id(doc_id_), score(score_)
    {}

    ~IndexResult(void)
    {}

    doc_id_t doc_id;
    score_t score;
    char *attr;
  };

  typedef std::vector<IndexResult> IndexResultSet;
  typedef IndexResultSet::iterator IRSItr;

}

#endif
