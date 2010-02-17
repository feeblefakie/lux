#include "boolean_query.h"

namespace Lux {

  BooleanQuery *BooleanQuery::create(bool is_composite)
  {
    BooleanQuery *boolean_query = new BooleanQuery(is_composite);
    return boolean_query;
  }

  BooleanQuery::~BooleanQuery(void)
  {
    QSCIterator itr_end = q_sets_.end();  
    for (QSCIterator itr = q_sets_.begin(); itr != itr_end; ++itr) {
      delete itr->query;
    }
  }

  IndexResultSet BooleanQuery::search(SearchIndex *si)
  {
    return si->search_boolean(this);
  }

  void BooleanQuery::add(Query *query, op_t op_type)
  {
    q_sets_.push_back(QuerySet(query, op_type));
  } 

  bool BooleanQuery::has_next(void)
  {
    return static_cast<const BooleanQuery&>(*this).has_next();
  }

  bool BooleanQuery::has_next(void) const
  {
    if (q_sets_iterator_ == q_sets_.end()) {
      return false;
    }
    return true;
  }

  QuerySet BooleanQuery::get_next(void)
  {
    return static_cast<const BooleanQuery&>(*this).get_next();
  }

  QuerySet BooleanQuery::get_next(void) const
  {
    return *q_sets_iterator_++;
  }

  std::string BooleanQuery::to_str(void)
  {
    // not implemented yet
  }

}
