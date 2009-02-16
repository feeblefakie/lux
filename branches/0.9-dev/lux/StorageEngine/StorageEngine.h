#ifndef LUX_STORAGE_ENGINE_H
#define LUX_STORAGE_ENGINE_H

#include "lux/lux.h"
#include "LIOBtree.h"
#include "LIOArray.h"
//#include "DepotHash.h"
//#include "CuriaHash.h"
//#include "VillaBtree.h"

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

//typedef Lux::StorageEngine<Lux::DepotHash> LuxHashStorage;
//typedef Lux::StorageEngine<Lux::CuriaHash> LuxDHashStorage;
//typedef Lux::StorageEngine<Lux::VillaBtree> LuxBtreeStorage;
typedef Lux::StorageEngine<Lux::LIOBtree> LuxBtreeStorage;
typedef Lux::StorageEngine<Lux::LIOArray> LuxArrayStorage;

#endif
