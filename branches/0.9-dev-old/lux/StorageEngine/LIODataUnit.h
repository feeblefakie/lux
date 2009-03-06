#ifndef LUX_LIO_DATAUNIT_H
#define LUX_LIO_DATAUNIT_H

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <stdint.h>

namespace Lux {

  class LIODataUnit {

  public:
    LIODataUnit(bool is_cleanup_needed = false)
    : is_cleanup_needed_(is_cleanup_needed)
    {}

    LIODataUnit(void *data, uint32_t size, bool is_cleanup_needed = false)
    : data_(data), size_(size), is_cleanup_needed_(is_cleanup_needed)
    {
      assert(data_ && size_ > 0);         
    } 

    void set_data(void *data)
    {
      assert(data);         
      data_ = data; 
    }

    void set_size(uint32_t size)
    {
      assert(size > 0);         
      size_ = size;
    }

    void set(void *data, uint32_t size)
    {
      assert(data && size > 0);         
      set_data(data);
      set_size(size);
    }

    void *get_data(void)
    {
      return data_;
    }

    uint32_t get_size(void)
    {
      return size_;
    }

    void set_cleanup_needed(bool is_cleanup_needed)
    {
      is_cleanup_needed_ = is_cleanup_needed;
    }

  protected:
    ~LIODataUnit() {
      if (is_cleanup_needed_) {     
        cleanup();
      }
    }

  private:
    void *data_;
    uint32_t size_;
    bool is_cleanup_needed_;

    void cleanup(void)
    {
      if (data_ != NULL) {
        delete [] (char *) data_;
        data_ = NULL;
      }
    }
  };
}

#endif
