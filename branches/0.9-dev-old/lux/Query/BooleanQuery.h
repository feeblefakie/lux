#ifndef LUX_BOOLEANQUERY_H
#define LUX_BOOLEANQUERY_H

#include "Query.h"
#include "lux/Storage/SearchIndex.h"
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
    static BooleanQuery *create(bool is_composite = false)
    {
      BooleanQuery *boolean_query = new BooleanQuery(is_composite);
      return boolean_query;
    }

    virtual ~BooleanQuery()
    {
      QSCIterator itr_end = q_sets_.end();  
      for (QSCIterator itr = q_sets_.begin(); itr != itr_end; ++itr) {
        delete itr->query;
      }
    }

    virtual IndexResultSet search(SearchIndex *si)
    {
      return si->search_boolean(this);
    }

    void add(Query *query, op_t op_type = OP_AND)
    {
      q_sets_.push_back(QuerySet(query, op_type));
    } 

    void set_composite(bool is_composite) { is_composite_ = is_composite; }
    bool is_composite(void) { return is_composite_; }
    bool is_composite(void) const { return is_composite_; }

    bool has_next()
    {
      return static_cast<const BooleanQuery&>(*this).has_next();
    }

    bool has_next() const
    {
      if (q_sets_iterator_ == q_sets_.end()) {
        return false;
      }
      return true;
    }

    QuerySet get_next()
    {
      return static_cast<const BooleanQuery&>(*this).get_next();
    }

    QuerySet get_next() const { return *q_sets_iterator_++; }

    void init_iter() { q_sets_iterator_ = q_sets_.begin(); }
    void init_iter() const { q_sets_iterator_ = q_sets_.begin(); }

    virtual std::string to_str(void)
    {
      // not implemented yet
    }

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
