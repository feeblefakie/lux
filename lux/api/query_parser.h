#ifndef LUX_API_QUERYPARSER_H
#define LUX_API_QUERYPARSER_H

#include <string>
#include "lux/query/phrase_query.h"
#include "lux/query/boolean_query.h"

namespace Lux {

  struct phrase_t {
    std::string index;
    std::string phrase;
  };

  /**
   * QueryParser
   */
  class QueryParser {
  public:
    /**
     * parse query
     * @param query pointer to query  
     */
    static Query *parse(const char *query);
    /**
     * parse query
     * @param query query object 
     */
    static Query *parse(std::string query);

  private:
    static phrase_t split_token_(std::string &token);

  };
}

#endif
