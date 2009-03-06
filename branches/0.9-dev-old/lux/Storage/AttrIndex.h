#ifndef LUX_ATTRINDEX_H
#define LUX_ATTRINDEX_H

#include "AttrIndexArray.h"

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
