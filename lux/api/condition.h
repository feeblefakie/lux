#ifndef LUX_API_CONDITION_H
#define LUX_API_CONDITION_H

#include "lux/lux.h"
#include "paging.h"
#include <string>

namespace Lux {

  class Paging;

  typedef enum {
    SORT_SCORE,
    SORT_ATTR_INT,
    SORT_ATTR_STR,
  } sort_attr_t;

  typedef enum {
    ASC,
    DESC,
  } sort_order_t;

  struct SortCondition {
    SortCondition(void) {}
    SortCondition(sort_attr_t _attr_type,
        sort_order_t _order_type, std::string _attr_name = "");
    ~SortCondition(void) {}

    sort_attr_t attr_type;
    sort_order_t order_type;
    std::string attr_name;
    uint32_t attr_size; // [NOTICE] set by system
  };

  struct Condition {
    Condition(void) {}
    Condition(SortCondition &_sort, Paging &_paging);
    ~Condition(void) {}

    SortCondition sort;
    Paging paging;
  };

}

#endif
