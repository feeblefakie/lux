#ifndef LUX_INDEX_ATTRINDEX_H
#define LUX_INDEX_ATTRINDEX_H

#include "attr_index_array.h"

namespace Lux {

  template <class AttrIndexPolicy>
  class AttrIndex : public AttrIndexPolicy {

  public:
    AttrIndex(Config::Document &doc_config)
    : AttrIndexPolicy(doc_config)
    {}
  };
}

typedef Lux::AttrIndex<Lux::AttrIndexArray> LuxAttrIndex;

#endif
