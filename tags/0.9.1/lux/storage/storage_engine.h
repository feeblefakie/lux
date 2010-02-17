#ifndef LUX_STORAGE_STORAGEENGINE_H
#define LUX_STORAGE_STORAGEENGINE_H

#include "lux/lux.h"
#include "luxio_btree.h"
#include "luxio_array.h"

namespace Lux {

  template <class StorageEnginePolicy>
  class StorageEngine : public StorageEnginePolicy {

  public:
    StorageEngine()
    : StorageEnginePolicy()
    {}

    StorageEngine(const char *filename, db_flags_t open_flags)
    : StorageEnginePolicy(filename, open_flags)
    {}

    StorageEngine(const std::string filename, db_flags_t open_flags)
    : StorageEnginePolicy(filename, open_flags)
    {}
  };
}

typedef Lux::StorageEngine<Lux::LuxIOBtree> LuxBtreeStorage;
typedef Lux::StorageEngine<Lux::LuxIOArray> LuxArrayStorage;

#endif
