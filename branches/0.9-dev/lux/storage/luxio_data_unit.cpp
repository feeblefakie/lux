#include "luxio_data_unit.h"
#include <cassert>

namespace Lux {

  LuxIODataUnit::LuxIODataUnit(bool is_cleanup_needed)
  : is_cleanup_needed_(is_cleanup_needed)
  {}

  LuxIODataUnit::LuxIODataUnit(void *data, uint32_t size, bool is_cleanup_needed)
  : data_(data), size_(size), is_cleanup_needed_(is_cleanup_needed)
  {
    assert(data_ && size_ > 0);         
  } 

  void LuxIODataUnit::set_data(void *data)
  {
    assert(data);         
    data_ = data; 
  }

  void LuxIODataUnit::set_size(uint32_t size)
  {
    assert(size > 0);         
    size_ = size;
  }

  void LuxIODataUnit::set(void *data, uint32_t size)
  {
    assert(data && size > 0);         
    set_data(data);
    set_size(size);
  }

  void *LuxIODataUnit::get_data(void)
  {
    return data_;
  }

  uint32_t LuxIODataUnit::get_size(void)
  {
    return size_;
  }

  void LuxIODataUnit::set_cleanup_needed(bool is_cleanup_needed)
  {
    is_cleanup_needed_ = is_cleanup_needed;
  }

  LuxIODataUnit::~LuxIODataUnit(void) {
    if (is_cleanup_needed_) {     
      cleanup();
    }
  }

  void LuxIODataUnit::cleanup(void)
  {
    if (data_ != NULL) {
      delete [] (char *) data_;
      data_ = NULL;
    }
  }

}
