#ifndef LUX_API_PAGING_H
#define LUX_API_PAGING_H

#include "lux/lux.h"

namespace Lux {

  /**
   * class Paging
   */
  class Paging {
  public:
    Paging(uint32_t cond_num = 10, uint32_t cond_page = 0);
    ~Paging() {}
    void set_total_num(uint32_t num_results);
    void calc(void);
    uint32_t get_base(void);
    uint32_t get_base(void) const;
    uint32_t get_num(void);
    uint32_t get_num(void) const;
    uint32_t get_page(void);
    uint32_t get_page(void) const;
    uint32_t get_total_num(void);
    uint32_t get_total_num(void) const;

  private:
    uint32_t total_num_;
    uint32_t num_;
    uint32_t page_;
    uint32_t base_;
  };
}

#endif
