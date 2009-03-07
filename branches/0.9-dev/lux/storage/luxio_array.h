#ifndef LUX_STORAGE_LUXIOARRAY_H
#define LUX_STORAGE_LUXIOARRAY_H

#include <luxio/array.h>
#include "lux/lux.h"
#include "lux/util.h"
#include "data_unit.h"
#include <cassert>
#include <string>

namespace Lux {

  class LuxIOArray {
  public:
    LuxIOArray(void);
    bool open(const char *filename, db_flags_t open_flags);
    bool close(void);
    void set_index_type(IO::db_index_t index_type,
                        uint8_t data_size = sizeof(uint32_t));
    bool put(uint32_t index, LuxDataUnit &val);
    bool put(uint32_t index, LuxDataUnit *val);
    bool append(uint32_t index, LuxDataUnit &val);
    bool append(uint32_t index, LuxDataUnit *val);
    bool get(uint32_t index, LuxDataUnit &val);
    bool get(uint32_t index, LuxDataUnit *val);
    bool del(uint32_t index);

  protected:
    ~LuxIOArray(void);

  private:
    IO::Array *ary_;
  };
}

#endif
