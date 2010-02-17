#ifndef LUX_API_CONDITION_H
#define LUX_API_CONDITION_H

#include "lux/lux.h"
#include "paging.h"
#include <string>

namespace Lux {

  class Paging;

  /**
   * sort attribute
   */
  typedef enum {
    SORT_SCORE,
    SORT_ATTR_INT,
    SORT_ATTR_STR,
  } sort_attr_t;

  /**
   * sort order
   */
  typedef enum {
    ASC,
    DESC,
  } sort_order_t;

  /**
   * condition in sorting
   */
  struct SortCondition {
    /**
     * constructor
     */
    SortCondition(void) {}
    /**
     * constructor
     * @param _attr_type attribute type in sort_attr_t
     * @param _order_type sort order in sort_order_t
     * @param _attr_name sort attribute name
     */
    SortCondition(sort_attr_t _attr_type,
        sort_order_t _order_type, std::string _attr_name = "");
    ~SortCondition(void) {}

    sort_attr_t attr_type;
    sort_order_t order_type;
    std::string attr_name;
    /**
     * @note this is set by the system. please do not set this directly
     */
    uint32_t attr_size; // [NOTICE] set by system
  };

  /**
   * search condition
   */
  struct Condition {
    /**
     * constructor
     */
    Condition(void) {}
    /**
     * constructor
     * @param _sort reference to SortCondition
     * @param -paging reference to Paging
     */
    Condition(SortCondition &_sort, Paging &_paging);
    /**
     * destructor
     */
    ~Condition(void) {}
    /**
     * SortCondition
     */
    SortCondition sort;
    /**
     * Paging
     */
    Paging paging;
  };

}

#endif
