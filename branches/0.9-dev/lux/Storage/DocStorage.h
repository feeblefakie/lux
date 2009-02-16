#ifndef LUX_DOCSTORAGE_H
#define LUX_DOCSTORAGE_H

#include "DocStorageArray.h"

namespace Lux {

  template <class DocStoragePolicy>
  class DocStorage : public DocStoragePolicy {

  public:
    DocStorage(Config::Document &doc_config)
    : DocStoragePolicy(doc_config)
    {}
  };
}

typedef Lux::DocStorage<Lux::DocStorageArray> LuxDocStorage;

#endif
