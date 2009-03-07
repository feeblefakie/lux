#ifndef LUX_STORAGE_DATAUNIT_H
#define LUX_STORAGE_DATAUNIT_H

#include "luxio_data_unit.h"

namespace Lux {

  template <class DataUnitPolicy>
  class DataUnit : public DataUnitPolicy {
  
  public:
    DataUnit(bool is_cleanup_needed = false)
    : DataUnitPolicy(is_cleanup_needed)
    {}

    DataUnit(void *data, unsigned int size, bool is_cleanup_needed = false)
    : DataUnitPolicy(data, size, is_cleanup_needed)
    {}
  };
}

typedef Lux::DataUnit<Lux::LuxIODataUnit> LuxDataUnit;

#endif
