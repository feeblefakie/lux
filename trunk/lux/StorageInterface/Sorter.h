#ifndef LUX_SORTER_H
#define LUX_SORTER_H

#include "lux/lux.h"
#include "Condition.h"
#include "lux/Storage/IndexResult.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

namespace Lux {

  template <class T>
  struct sort_by_score_desc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return t1.score > t2.score;
      }
  };
  template <class T>
  struct sort_by_score_asc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return t1.score < t2.score;
      }
  };
  template <class T>
  struct sort_by_attr_desc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return t1.attr > t2.attr;
      }
  };
  template <class T>
  struct sort_by_attr_asc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return t1.attr < t2.attr;
      }
  };

  class Sorter {

  public:
    Sorter(sort_t sort_type, uint_t num_of_sorted)
    : sort_type_(sort_type), num_of_sorted_(num_of_sorted)
    {}
    ~Sorter(void) {}
    void sort(IndexResultSet &rs)
    {
      /**
       * [TODO] BAD CODE. only functor should be selected in the case clause.
       */
      switch (sort_type_) {
        case SORT_SCORE_ASC:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_score_asc<IndexResult>());
          break;
        case SORT_SCORE_DESC:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_score_desc<IndexResult>());
          break;
        case SORT_ATTR_DESC:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_attr_desc<IndexResult>());
          break;
        case SORT_ATTR_ASC:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_attr_asc<IndexResult>());
          break;
        default:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_score_desc<IndexResult>());
      }
    }

  private:
    sort_t sort_type_;
    uint_t num_of_sorted_;
  };

}

#endif
