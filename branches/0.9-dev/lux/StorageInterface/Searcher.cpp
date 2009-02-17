#include "Searcher.h"
#include "Condition.h"
#include "Sorter.h"
#include "QueryParser.h"
#include "lux/Document/Document.h"
#include "lux/Storage/InvertedIndex.h"

namespace Lux {

  Searcher::Searcher(Engine &engine)
  : engine_(engine)
  {
    engine_.si->set_sys_cond(RES_W_POS);
  }

  Searcher::~Searcher()
  {}

  void Searcher::set_sys_cond(sys_cond_t sys_cond)
  {
    engine_.si->set_sys_cond(sys_cond);
  }

  ResultSet Searcher::search(const char *query, Condition &cond)
  {
    return search(std::string(query), cond);
  }

  ResultSet Searcher::search(const std::string &query, Condition &cond)
  {
    IndexResultSet irs = search_index(query);

    cond.paging.set_total_num(irs.size());
    cond.paging.calc();

    if (irs.size() <= 0) { return ResultSet(cond.paging); }

    set_attrs(irs, cond); // set if specified

    Sorter<IndexResult> sorter(cond.sort, 
                               cond.paging.get_base() + cond.paging.get_num());
    sorter.sort(irs);

    clear_attrs(irs, cond); // clear if specified

    ResultSet rs(cond.paging);
    // [FIXME] adding doc_id only and get contents everytime rs.get_next() called
    for (int i = rs.get_base(); i < rs.get_base() + rs.get_num(); ++i) {
      Document doc = engine_.ds->get(irs[i].doc_id);
      if (cond.sort.attr_type == SORT_SCORE) {
        rs.add(Result(doc, irs[i].score)); // set score only when sorted by score
      } else {
        rs.add(Result(doc));
      }
    }

    return rs;
  }

  IndexResultSet Searcher::search_by_server(const char *query, Condition &cond)
  {
    return search_by_server(std::string(query), cond);
  }

  IndexResultSet Searcher::search_by_server(const std::string &query, Condition &cond)
  {
    IndexResultSet irs = search_index(query);

    cond.paging.set_total_num(irs.size());
    cond.paging.calc();

    if (irs.size() <= 0) { return irs; }

    set_attrs(irs, cond); // set if specified

    Sorter<IndexResult> sorter(cond.sort,
        cond.paging.get_base() + cond.paging.get_num());
    sorter.sort(irs);

    // [BAD] attributes must be cleared by servers

    return irs;
  }

  std::string Searcher::getdoc_by_server(doc_id_t doc_id)
  {
    return engine_.ds->get_raw(doc_id);
  }

  IndexResultSet Searcher::search_index(const std::string &query)
  {
    Query *q = QueryParser::parse(query);
    IndexResultSet irs = engine_.si->search(q);
    delete q;  
    return irs;
  }

  void Searcher::set_attrs(IndexResultSet &irs, Condition &cond)
  {
    if (cond.sort.attr_type == SORT_SCORE) {
      return; // nothing to do
    }
    AttrIndexEngine aie(cond.sort.attr_name);
    if (!engine_.ai->get(aie)) {
      error_log("no such attribute index");
      cond.sort.attr_type = SORT_SCORE;
      cond.sort.order_type = DESC;
      return;
    }

    // [BAD] attr_size is refered by servers
    cond.sort.attr_size = aie.attr_size;

    IRSItr itr_end = irs.end();
    for (IRSItr itr = irs.begin(); itr != itr_end; ++itr) {
      LuxDataUnit attr;
      if (aie.engine->get(itr->doc_id, attr)) {
        attr.set_cleanup_needed(false);
        itr->attr = (char *) attr.get_data();
      }
    }
  }

  void Searcher::clear_attrs(IndexResultSet &irs, Condition &cond)
  {
    if (cond.sort.attr_type == SORT_SCORE) { return; }
    IRSItr itr_end = irs.end();
    for (IRSItr itr = irs.begin(); itr != itr_end; ++itr) {
      delete [] itr->attr;
    }
  }

}
