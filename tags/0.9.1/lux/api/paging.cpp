#include "paging.h"

namespace Lux {

  Paging::Paging(uint32_t cond_num, uint32_t cond_page)
  : total_num_(0), num_(cond_num), page_(cond_page), base_(0)
  {}

  void Paging::set_total_num(uint32_t num_results)
  {
    total_num_ = num_results;
  }

  void Paging::calc(void)
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

  uint32_t Paging::get_base(void)
  { 
    return static_cast<const Paging&>(*this).get_base();
  }

  uint32_t Paging::get_base(void) const
  {
    return base_;
  }

  uint32_t Paging::get_num(void)
  { 
    return static_cast<const Paging&>(*this).get_num();
  }

  uint32_t Paging::get_num(void) const
  {
    return num_;
  }

  uint32_t Paging::get_page(void)
  {
    return static_cast<const Paging&>(*this).get_page();
  }

  uint32_t Paging::get_page(void) const
  {
    return page_;
  }

  uint32_t Paging::get_total_num(void)
  { 
    return static_cast<const Paging&>(*this).get_total_num();
  }

  uint32_t Paging::get_total_num(void) const
  {
    return total_num_;
  }

}
