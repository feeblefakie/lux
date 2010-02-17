#ifndef LUX_STORAGE_LUXIOBTREE_H
#define LUX_STORAGE_LUXIOBTREE_H

#include <luxio/btree.h>
#include "lux/lux.h"
#include "lux/util.h"
#include "data_unit.h"
#include <cassert>
#include <iostream>
#include <string>

namespace Lux {

  class LuxIOBtree {
  public:
    LuxIOBtree(void);
    bool open(const char *filename, db_flags_t open_flags);
    bool close(void);
    void set_index_type(IO::db_index_t index_type);
    void set_cmp_func(IO::CMP cmp);
    bool put(LuxDataUnit &key, LuxDataUnit &val);
    bool put(LuxDataUnit *key, LuxDataUnit *val);
    bool append(LuxDataUnit &key, LuxDataUnit &val);
    bool append(LuxDataUnit *key, LuxDataUnit *val);
    bool get(LuxDataUnit &key, LuxDataUnit &val);
    bool get(LuxDataUnit *key, LuxDataUnit *val);
    bool del(LuxDataUnit &key);
    bool del(LuxDataUnit *key);

  protected:
    ~LuxIOBtree(void);

  private:
    IO::Btree *bt_;
  };

}

#endif
