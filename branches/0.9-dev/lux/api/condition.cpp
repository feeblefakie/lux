#include "condition.h"
#include "lux/util.h"

namespace Lux {

  SortCondition::SortCondition(sort_attr_t _attr_type,
                               sort_order_t _order_type, 
                               std::string _attr_name)
  : attr_type(_attr_type),
    order_type(_order_type),
    attr_name(_attr_name),
    attr_size(sizeof(score_t))
  {
    if (attr_name.empty() && attr_type != SORT_SCORE) {
      error_log("attribute is unspecified. sorting by score.");
      attr_type = SORT_SCORE;
      order_type = DESC;
    }
  }

  Condition::Condition(SortCondition &_sort, Paging &_paging)
  : sort(_sort), paging(_paging)
  {}

}
