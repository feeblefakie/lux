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

  class QueryParser {
  public:
    static Query *parse(const char *query);
    static Query *parse(std::string query);

  private:
    static phrase_t split_token_(std::string &token);

  };
}

#endif
