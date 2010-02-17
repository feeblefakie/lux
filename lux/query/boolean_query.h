#ifndef LUX_QUERY_BOOLEANQUERY_H
#define LUX_QUERY_BOOLEANQUERY_H

#include "query.h"
#include "lux/index/search_index.h"
#include <vector>
#include <iostream>

namespace Lux {

  typedef enum {
    OP_AND = 1,
    OP_OR = 2,
    OP_NOT = 3,
  } op_t;

  struct QuerySet {
    QuerySet(Query *query_, int op_type_)
    : query(query_), op_type(op_type_)
    {}
    Query *query;
    int op_type;
  };

  typedef std::vector<QuerySet> QSContainer;
  typedef QSContainer::const_iterator QSCIterator;

  class BooleanQuery : public Query {
  public:
    static BooleanQuery *create(bool is_composite = false);
    virtual ~BooleanQuery(void);
    virtual IndexResultSet search(SearchIndex *si);
    void add(Query *query, op_t op_type = OP_AND);
    void set_composite(bool is_composite) { is_composite_ = is_composite; }
    bool is_composite(void) { return is_composite_; }
    bool is_composite(void) const { return is_composite_; }
    bool has_next(void);
    bool has_next(void) const;
    QuerySet get_next(void);
    QuerySet get_next(void) const;
    void init_iter(void) { q_sets_iterator_ = q_sets_.begin(); }
    void init_iter(void) const { q_sets_iterator_ = q_sets_.begin(); }
    virtual std::string to_str(void);

  private:
    QSContainer q_sets_;
    mutable QSCIterator q_sets_iterator_;
    bool is_composite_;

    BooleanQuery(bool is_composite = false)
    : is_composite_(is_composite)
    {}
  };
}

#endif
