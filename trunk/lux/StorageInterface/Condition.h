#ifndef LUX_CONDITION_H
#define LUX_CONDITION_H

#include "lux/lux.h"

namespace Lux {

  typedef enum {
    SORT_SCORE_ASC = 1,
    SORT_SCORE_DESC = 2,
    SORT_ATTR_DESC = 3,
    SORT_ATTR_ASC = 4,
  } sort_t;

  class Condition {

  public:
    Condition(void) {}
    Condition(sort_t _sort, uint_t _num = 10, uint_t _page = 0)
    : sort(_sort), num(_num), page(_page) {}
    ~Condition(void) {}

    sort_t sort;
    uint_t num;
    uint_t page;

  };
}

#endif
