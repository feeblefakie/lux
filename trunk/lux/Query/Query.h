#ifndef LUX_QUERY_H
#define LUX_QUERY_H

#include <string>
#include "lux/Storage/IndexResult.h"

namespace Lux {
    
  class SearchIndex;

  class Query {

  public:
    virtual ~Query() {};
    virtual IndexResultSet search(SearchIndex *si) = 0;
    void set_boost(float boost) { boost = boost; }
    float get_boost(void) const { return boost; }
    virtual std::string to_str(void) = 0;

  private:
    float boost;
  };
 
}

#endif
