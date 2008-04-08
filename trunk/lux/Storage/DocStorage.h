#ifndef LUX_DOCSTORAGE_H
#define LUX_DOCSTORAGE_H

#include "DocStorageHash.h"

namespace Lux {

  template <class DocStoragePolicy>
  class DocStorage : public DocStoragePolicy {

  public:
    DocStorage(std::string storage_dir, db_flags_t open_params,
               DocumentDefinition &doc_def)
    : DocStoragePolicy(storage_dir, open_params, doc_def)
    {}
  };
}

typedef Lux::DocStorage<Lux::DocStorageHash> LuxDocStorage;

#endif
