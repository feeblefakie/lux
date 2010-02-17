#ifndef LUX_API_PAGING_H
#define LUX_API_PAGING_H

#include "lux/lux.h"

namespace Lux {

  /**
   * paging interface
   */
  class Paging {
  public:
    /**
     * constructor
     * @param cond_num the number of results returned
     * @param cond_page page offset
     */
    Paging(uint32_t cond_num = 10, uint32_t cond_page = 0);
    /**
     * destructor
     */
    ~Paging() {}
    /**
     * set the total number of results returned
     * @param num_results the total number of results returned
     * @note this is a method used internally
     */
    void set_total_num(uint32_t num_results);
    /**
     * calculate the offset of the returned results (base_) 
     * and the number of results returned (num_)
     */
    void calc(void);
    /**
     * get page offset
     */
    uint32_t get_base(void);
    /**
     * get page offset
     */
    uint32_t get_base(void) const;
    /**
     * get the number of results returned
     */
    uint32_t get_num(void);
    /**
     * get the number of results returned
     */
    uint32_t get_num(void) const;
    /**
     * get page offset
     */
    uint32_t get_page(void);
    /**
     * get page offset
     */
    uint32_t get_page(void) const;
    /**
     * get the total number of results
     */
    uint32_t get_total_num(void);
    /**
     * get the total number of results
     */
    uint32_t get_total_num(void) const;

  private:
    uint32_t total_num_;
    uint32_t num_;
    uint32_t page_;
    uint32_t base_;
  };
}

#endif
