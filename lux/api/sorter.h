#ifndef LUX_API_SORTER_H
#define LUX_API_SORTER_H

#include "lux/lux.h"
#include "condition.h"
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
  struct sort_by_attr_int_desc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return *(int *) t1.attr > *(int *) t2.attr;
      }
  };
  template <class T>
  struct sort_by_attr_int_asc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return *(int *) t1.attr < *(int *) t2.attr;
      }
  };
  template <class T>
  struct sort_by_attr_str_desc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return strcmp((char *) t1.attr, (char *) t2.attr) > 0;
      }
  };
  template <class T>
  struct sort_by_attr_str_asc : public std::binary_function<T, T, bool>
  {
      bool operator()(T &t1, T &t2) {
        return strcmp((char *) t1.attr, (char *) t2.attr) < 0;
      }
  };

  template <class T>
  class Sorter {

  public:
    Sorter(SortCondition sort, uint32_t num_of_sorted)
    : sort_(sort), num_of_sorted_(num_of_sorted)
    {}
    ~Sorter(void) {}
    void sort(std::vector<T> &rs)
    {
      /**
       * [TODO] BAD CODE. only functor should be selected in the case clause.
       */
      switch (sort_.attr_type) {
        case SORT_SCORE:
          if (sort_.order_type == DESC) {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_score_desc<T>());
          } else {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_score_asc<T>());
          }
          break;
        case SORT_ATTR_INT:
          if (sort_.order_type == DESC) {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_attr_int_desc<T>());
          } else {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_attr_int_asc<T>());
          }
          break;
        case SORT_ATTR_STR:
          if (sort_.order_type == DESC) {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_attr_str_desc<T>());
          } else {
            partial_sort(rs.begin(),
                         rs.begin() + num_of_sorted_,
                         rs.end(),
                         sort_by_attr_str_asc<T>());
          }
          break;
        default:
          partial_sort(rs.begin(),
                       rs.begin() + num_of_sorted_,
                       rs.end(),
                       sort_by_score_desc<T>());
      }
    }

  private:
    SortCondition sort_;
    uint32_t num_of_sorted_;
  };

}

#endif
