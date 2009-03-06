#ifndef LUX_RESULT_H
#define LUX_RESULT_H

#include "lux/lux.h"
#include "Paging.h"
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
    Result(Document &doc)
    : doc_(doc), score_(0)
    {}

    Result(Document &doc, score_t score)
    : doc_(doc), score_(score)
    {}

    ~Result() {}

    std::string get_id(void)
    {
      return doc_.get_eid(); 
    }

    std::string get(std::string field_name)
    {
      return doc_.get_by_name(field_name);
    }

    score_t get_score(void)
    {
      return score_;
    }

  private:
    Document doc_;
    score_t score_;
  };

  typedef std::vector<Result> ResultList;
  typedef ResultList::iterator RLIterator;

  /**
   * class ResultSet
   */
  class ResultSet {

  public:
    ResultSet(void)
    {}

    ResultSet(uint32_t total_num, uint32_t base, uint32_t num)
    : total_num_(total_num), base_(base), num_(num)
    {}

    ResultSet(Paging &paging)
    : total_num_(paging.get_total_num()),
      base_(paging.get_base()),
      num_(paging.get_num())
    {}

    ~ResultSet()
    {}

    void set_total_num(uint32_t total_num)
    {
      total_num_ = total_num;
    }

    void set_base(uint32_t base)
    {
      base_ = base;
    }

    void set_num(uint32_t num)
    {
      num_ = num;
    }

    uint32_t get_base(void)
    { 
      return static_cast<const ResultSet&>(*this).get_base();
    }

    uint32_t get_base(void) const
    {
      return base_;
    }

    uint32_t get_num(void)
    { 
      return static_cast<const ResultSet&>(*this).get_num();
    }

    uint32_t get_num(void) const
    {
      return num_;
    }

    uint32_t get_total_num(void)
    { 
      return static_cast<const ResultSet&>(*this).get_total_num();
    }

    uint32_t get_total_num(void) const
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
    uint32_t total_num_;
    uint32_t base_;
    uint32_t num_;
  };
}

#endif
