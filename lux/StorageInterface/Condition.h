#ifndef LUX_CONDITION_H
#define LUX_CONDITION_H

#include "lux/lux.h"
#include "Paging.h"

namespace Lux {

  // [TODO] SortField class ? 
  // setting sort field and type and order etc ...

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
    SortCondition(void)
    {}
    SortCondition(sort_attr_t _attr_type,
        sort_order_t _order_type, std::string _attr_name = "")
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
    ~SortCondition(void)
    {}
    sort_attr_t attr_type;
    sort_order_t order_type;
    std::string attr_name;
    uint32_t attr_size; // [NOTICE] set by system
  };

  struct Condition {
    Condition(void)
    {}
    Condition(SortCondition _sort, Paging _paging)
    : sort(_sort), paging(_paging)
    {}
    ~Condition(void)
    {}

    SortCondition sort;
    Paging paging;
  };

}

#endif
