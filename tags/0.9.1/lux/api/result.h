#ifndef LUX_API_RESULT_H
#define LUX_API_RESULT_H

#include "lux/lux.h"
#include "lux/document/document.h"
#include <string>
#include <vector>

namespace Lux {

  class Paging;

  /**
   * class Result
   */
  class Result {
  public:
    Result(Document &doc);
    Result(Document &doc, score_t score);
    ~Result() {}
    std::string get_id(void);
    std::string get(std::string field_name);
    score_t get_score(void);

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
    ResultSet(void) {}
    ResultSet(uint32_t total_num, uint32_t base, uint32_t num);
    ResultSet(Paging &paging);
    ~ResultSet() {}
    void set_total_num(uint32_t total_num);
    void set_base(uint32_t base);
    void set_num(uint32_t num);
    uint32_t get_base(void);
    uint32_t get_base(void) const;
    uint32_t get_num(void);
    uint32_t get_num(void) const;
    uint32_t get_total_num(void);
    uint32_t get_total_num(void) const;
    void add(Result result);
    bool has_next(void);
    bool has_next(void) const;
    const Result get_next(void);
    const Result get_next(void) const;
    void init_iter(void);

  private:
    ResultList result_list_;
    mutable RLIterator rl_iterator_; 
    uint32_t total_num_;
    uint32_t base_;
    uint32_t num_;
  };

}

#endif
