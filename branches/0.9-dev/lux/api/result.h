#ifndef LUX_API_RESULT_H
#define LUX_API_RESULT_H

#include "lux/lux.h"
#include "lux/document/document.h"
#include <string>
#include <vector>

namespace Lux {

  class Paging;

  /**
   * Result
   */
  class Result {
  public:
    /**
     * constructor
     * @param doc reference to Document
     */
    Result(Document &doc);
    /**
     * constructor
     * @param doc reference to Document
     * @param score score structure
     */
    Result(Document &doc, score_t score);
    /**
     * destructor
     */
    ~Result() {}
    /**
     * get the result document ID
     */
    std::string get_id(void);
    /**
     * get the field value
     */
    std::string get(std::string field_name);
    /**
     * get the score structure
     */
    score_t get_score(void);

  private:
    Document doc_;
    score_t score_;
  };

  typedef std::vector<Result> ResultList;
  typedef ResultList::iterator RLIterator;

  /**
   * ResultSet
   */
  class ResultSet {
  public:
    /**
     * constructor
     */
    ResultSet(void) {}
    /**
     * constructor
     * @param total_num the total number of results returned
     * @param base results offset
     * @param num the number of results to be returned
     */
    ResultSet(uint32_t total_num, uint32_t base, uint32_t num);
    /**
     * constructor
     * @param paging reference to Paging
     */
    ResultSet(Paging &paging);
    /**
     * destructor
     */
    ~ResultSet() {}
    /**
     * @note it is used internally
     */
    void set_total_num(uint32_t total_num);
    /**
     * @note it is used internally
     */
    void set_base(uint32_t base);
    /**
     * @note it is used internally
     */
    void set_num(uint32_t num);
    /**
     * @note it is used internally
     */
    uint32_t get_base(void);
    /**
     * @note it is used internally
     */
    uint32_t get_base(void) const;
    /**
     * @note it is used internally
     */
    uint32_t get_num(void);
    /**
     * @note it is used internally
     */
    uint32_t get_num(void) const;
    /**
     * @note it is used internally
     */
    uint32_t get_total_num(void);
    /**
     * @note it is used internally
     */
    uint32_t get_total_num(void) const;
    /**
     * @note it is used internally
     */
    void add(Result result);
    /**
     * @note it is used internally
     */
    bool has_next(void);
    /**
     * @note it is used internally
     */
    bool has_next(void) const;
    /**
     * @note it is used internally
     */
    const Result get_next(void);
    /**
     * @note it is used internally
     */
    const Result get_next(void) const;
    /**
     * @note it is used internally
     */
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
