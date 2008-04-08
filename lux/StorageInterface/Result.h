#ifndef LUX_RESULT_H
#define LUX_RESULT_H

#include "lux/lux.h"
#include "lux/Document/Document.h"
#include <string>
#include <vector>
#include <iostream>

namespace Lux {

  /**
   * class Result
   */
  class Result {

  public:
    Result(Document &doc, uint_t score)
    : doc_(doc), score_(score)
    {}
    ~Result() {}

    std::string get_id(void)
    {
      return doc_.get_external_id(); 
    }

    std::string get(std::string field_name)
    {
      return doc_.get_by_name(field_name);
    }

    uint_t get_score(void)
    {
      return score_;
    }

  private:
    Document doc_;
    uint_t score_;
  };

  typedef std::vector<Result> ResultList;
  typedef ResultList::iterator RLIterator;

  /**
   * class ResultSet
   */
  class ResultSet {

  public:
    ResultSet()
    {
      base_ = 0;
      num_ = 0;
      total_num_ = 0;
    }
    ~ResultSet() {}

    void set_result_info(uint_t cond_num, uint_t cond_page, uint_t num_of_result)
    {
      total_num_ = num_of_result;
      uint_t req_num_base = cond_num * cond_page;
      uint_t req_num = req_num_base + cond_num;

      if (num_of_result > req_num) {
        base_ = cond_num * cond_page;
        num_ = cond_num;
      } else if (num_of_result > req_num_base) {
        base_ = req_num_base;
        num_ = num_of_result - req_num_base;
      } else if (num_of_result > cond_num) {
        base_ = 0;
        num_ = cond_num; 
      } else {
        base_ = 0;
        num_ = num_of_result;
      }
    }

    uint_t get_base(void)
    { 
      return static_cast<const ResultSet&>(*this).get_base();
    }

    uint_t get_base(void) const
    {
      return base_;
    }

    uint_t get_num(void)
    { 
      return static_cast<const ResultSet&>(*this).get_num();
    }

    uint_t get_num(void) const
    {
      return num_;
    }

    uint_t get_total_num(void)
    { 
      return static_cast<const ResultSet&>(*this).get_total_num();
    }

    uint_t get_total_num(void) const
    {
      return total_num_;
    }

    void add(Result result)
    {
      result_list_.push_back(result);
    }

    bool has_next(void)
    {
      return static_cast<const ResultSet&>(*this).has_next();
    }

    bool has_next(void) const
    {
      if (rl_iterator_ == result_list_.end()) {
        return false;
      }
      return true;
    }

    const Result get_next(void)
    { 
      return static_cast<const ResultSet&>(*this).get_next();
    }

    const Result get_next(void) const
    { 
      return *rl_iterator_++;
    }

    void init_iter(void)
    { 
      rl_iterator_ = result_list_.begin();
    }

  private:
    ResultList result_list_;
    mutable RLIterator rl_iterator_; 
    uint_t base_; // where the result from
    uint_t num_;  // the number of results actually retrieved
    uint_t total_num_;
  };
}

#endif
