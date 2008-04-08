#ifndef LUX_SEARCHER_H
#define LUX_SEARCHER_H

#include "lux/lux.h"
#include "Result.h"
#include "lux/Storage/DocStorage.h"
#include "lux/StorageEngine/StorageEngine.h"
#include "lux/scoped_ptr.h"
#include <string>

namespace Lux {
  
  class Condition;
  class SearchIndex;
  class DocumentDefinition;

  class Searcher {

  public:
    Searcher(std::string storage_dir, DocumentDefinition &doc_def);
    ~Searcher(); 
    ResultSet search(const char *query, Condition &cond);
    ResultSet search(std::string query, Condition &cond);

  private:
    scoped_ptr<LuxHashStorage> lhs_;
    scoped_ptr<LuxDocStorage> ds_;
    scoped_ptr<SearchIndex> si_;
  };
}

#endif
