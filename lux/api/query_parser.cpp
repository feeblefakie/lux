#include "query_parser.h"

namespace Lux {

  Query *QueryParser::parse(const char *query)
  {
    return parse(std::string(query)); 
  }

  Query *QueryParser::parse(std::string query)
  {
    BooleanQuery *bq = BooleanQuery::create();
    std::string cur_token = "";
    for(int i = 0, n; i <= query.length(); i = n+1) {
      n = query.find_first_of(" ", i);
      if (n == std::string::npos) {
        n = query.length();
      }
      std::string token = query.substr(i, n-i);
      //std::cout << token << std::endl;

      if (token == "AND" || token == "OR" || token == "NOT") {
        cur_token = token;
      } else {
        phrase_t split_token = split_token_(token);
        //std::cout << "index[" << split_token.index << "]" << std::endl;
        //std::cout << "phrase[" << split_token.phrase << "]" << std::endl;
        if (split_token.phrase.empty()) {
          continue;
        }
        PhraseQuery *q = PhraseQuery::create(split_token.phrase,
                                             split_token.index);
        if (cur_token == "AND") {
          bq->add(q, OP_AND); 
        } else if (cur_token == "OR") {
          bq->add(q, OP_OR); 
        } else if (cur_token == "NOT") {
          bq->add(q, OP_NOT); 
        } else {
          bq->add(q); 
        }
        cur_token = "";
      }
    }
    return bq;
  }

  /**
   * [TODO] try to find a way to deal with ':' which is not for separting token
   */
  phrase_t QueryParser::split_token_(std::string &token)
  {
    phrase_t split_token;
    int pos = 0;
    int n = token.find_first_of(":", pos);
    if(n == std::string::npos) {
      split_token.index = "default";
      split_token.phrase = token;
      return split_token;
    }
    split_token.index  = token.substr(pos, n-pos);
    split_token.phrase  = token.substr(n+1);
    return split_token;
  }

}
