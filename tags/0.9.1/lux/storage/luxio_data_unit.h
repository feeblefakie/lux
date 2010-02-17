#ifndef LUX_STORAGE_LUXIODATAUNIT_H
#define LUX_STORAGE_LUXIODATAUNIT_H

#include <cstdlib>
#include <stdint.h>

namespace Lux {

  class LuxIODataUnit {
  public:
    LuxIODataUnit(bool is_cleanup_needed = false);
    LuxIODataUnit(void *data, uint32_t size, bool is_cleanup_needed = false);
    void set_data(void *data);
    void set_size(uint32_t size);
    void set(void *data, uint32_t size);
    void *get_data(void);
    uint32_t get_size(void);
    void set_cleanup_needed(bool is_cleanup_needed);

  protected:
    ~LuxIODataUnit(void);

  private:
    void *data_;
    uint32_t size_;
    bool is_cleanup_needed_;

    void cleanup(void);
  };
}

#endif
