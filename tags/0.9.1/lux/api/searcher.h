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

  class Searcher {

  public:
    Searcher(Engine &engine);
    ~Searcher(); 
    bool open(void);
    bool close(void);
    void set_sys_cond(sys_cond_t sys_cond);
    ResultSet search(const char *query, Condition &cond);
    ResultSet search(const std::string &query, Condition &cond);
    IndexResultSet search_by_server(const char *query, Condition &cond);
    IndexResultSet search_by_server(const std::string &query, Condition &cond);
    std::string getdoc_by_server(doc_id_t doc_id);
    // [FIXME] this shouldn't be public. (needed by server)
    void clear_attrs(IndexResultSet &irs, Condition &cond);

  private:
    Engine engine_;

    IndexResultSet search_index(const std::string &query);
    void set_attrs(IndexResultSet &irs, Condition &cond);
  };
}

#endif
