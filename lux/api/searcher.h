#ifndef LUX_API_SEARCHER_H
#define LUX_API_SEARCHER_H

#include "lux/lux.h"
#include "result.h"
#include "engine.h"
#include "lux/index/index_result.h"
#include <string>

namespace Lux {
  
  class Condition;
  class DocumentDefinition;

  /**
   * searcher interface
   */
  class Searcher {

  public:
    /**
     * constructor
     * @param engine database engine object
     */
    Searcher(Engine &engine);
    /**
     * destructor
     */
    ~Searcher(); 
    /**
     * set system condition 
     * @param sys_cond system condition such as an index format
     */
    void set_sys_cond(sys_cond_t sys_cond);
    /**
     * search query
     * @param query pointer to query
     * @param cond reference to Condition
     */
    ResultSet search(const char *query, Condition &cond);
    /**
     * search query
     * @param query reference to query
     * @param cond reference to Condition
     */
    ResultSet search(const std::string &query, Condition &cond);
    /**
     * search query by internal server
     * @note do not use this method directory. 
     */
    IndexResultSet search_by_server(const char *query, Condition &cond);
    /**
     * search query by internal server
     * @note do not use this method directory. 
     */
    IndexResultSet search_by_server(const std::string &query, Condition &cond);
    /**
     * get document by internal server
     * @note do not use this method directory. 
     */
    std::string getdoc_by_server(doc_id_t doc_id);
    /**
     * @note FIXME: this shouldn't be public. (needed by internal server for now)
     */
    void clear_attrs(IndexResultSet &irs, Condition &cond);

  private:
    Engine engine_;

    IndexResultSet search_index(const std::string &query);
    void set_attrs(IndexResultSet &irs, Condition &cond);
  };
}

#endif
