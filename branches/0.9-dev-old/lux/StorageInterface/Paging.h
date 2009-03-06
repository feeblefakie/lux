#ifndef LUX_PAGING_H
#define LUX_PAGING_H

#include "lux/lux.h"

namespace Lux {

  /**
   * class Paging
   */
  class Paging {

  public:
    Paging(uint32_t cond_num = 10, uint32_t cond_page = 0)
    : total_num_(0), num_(cond_num), page_(cond_page), base_(0)
    {}

    ~Paging()
    {}

    void set_total_num(uint32_t num_results)
    {
      total_num_ = num_results;
    }

    void calc(void)
    {
      uint32_t req_num_base = num_ * page_;
      uint32_t req_num = req_num_base + num_;

      if (total_num_ > req_num) {
        base_ = num_ * page_;
      } else if (total_num_ > req_num_base) {
        base_ = req_num_base;
        num_ = total_num_ - req_num_base;
      } else if (total_num_ > num_) {
        base_ = 0;
      } else {
        base_ = 0;
        num_ = total_num_;
      }
    }

    uint32_t get_base(void)
    { 
      return static_cast<const Paging&>(*this).get_base();
    }

    uint32_t get_base(void) const
    {
      return base_;
    }

    uint32_t get_num(void)
    { 
      return static_cast<const Paging&>(*this).get_num();
    }

    uint32_t get_num(void) const
    {
      return num_;
    }

    uint32_t get_page(void)
    {
      return static_cast<const Paging&>(*this).get_page();
    }

    uint32_t get_page(void) const
    {
      return page_;
    }

    uint32_t get_total_num(void)
    { 
      return static_cast<const Paging&>(*this).get_total_num();
    }

    uint32_t get_total_num(void) const
    {
      return total_num_;
    }

  private:
    uint32_t total_num_;
    uint32_t num_;
    uint32_t page_;
    uint32_t base_;
  };
}

#endif
