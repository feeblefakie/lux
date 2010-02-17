#include "result.h"
#include "paging.h"

namespace Lux {

  /**
   * class Result
   */
  Result::Result(Document &doc)
  : doc_(doc), score_(0)
  {}

  Result::Result(Document &doc, score_t score)
  : doc_(doc), score_(score)
  {}

  std::string Result::get_id(void)
  {
    return doc_.get_eid(); 
  }

  std::string Result::get(std::string field_name)
  {
    return doc_.get_by_name(field_name);
  }

  score_t Result::get_score(void)
  {
    return score_;
  }

  /**
   * class ResultSet
   */
  ResultSet::ResultSet(uint32_t total_num, uint32_t base, uint32_t num)
  : total_num_(total_num), base_(base), num_(num)
  {}

  ResultSet::ResultSet(Paging &paging)
  : total_num_(paging.get_total_num()),
    base_(paging.get_base()),
    num_(paging.get_num())
  {}

  void ResultSet::set_total_num(uint32_t total_num)
  {
    total_num_ = total_num;
  }

  void ResultSet::set_base(uint32_t base)
  {
    base_ = base;
  }

  void ResultSet::set_num(uint32_t num)
  {
    num_ = num;
  }

  uint32_t ResultSet::get_base(void)
  { 
    return static_cast<const ResultSet&>(*this).get_base();
  }

  uint32_t ResultSet::get_base(void) const
  {
    return base_;
  }

  uint32_t ResultSet::get_num(void)
  { 
    return static_cast<const ResultSet&>(*this).get_num();
  }

  uint32_t ResultSet::get_num(void) const
  {
    return num_;
  }

  uint32_t ResultSet::get_total_num(void)
  { 
    return static_cast<const ResultSet&>(*this).get_total_num();
  }

  uint32_t ResultSet::get_total_num(void) const
  {
    return total_num_;
  }

  void ResultSet::add(Result result)
  {
    result_list_.push_back(result);
  }

  bool ResultSet::has_next(void)
  {
    return static_cast<const ResultSet&>(*this).has_next();
  }

  bool ResultSet::has_next(void) const
  {
    if (rl_iterator_ == result_list_.end()) {
      return false;
    }
    return true;
  }

  const Result ResultSet::get_next(void)
  { 
    return static_cast<const ResultSet&>(*this).get_next();
  }

  const Result ResultSet::get_next(void) const
  { 
    return *rl_iterator_++;
  }

  void ResultSet::init_iter(void)
  { 
    rl_iterator_ = result_list_.begin();
  }

}
