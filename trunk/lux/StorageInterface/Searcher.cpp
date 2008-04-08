#include "Searcher.h"
#include "Condition.h"
#include "Sorter.h"
#include "QueryParser.h"
#include "lux/Document/Document.h"
#include "lux/Storage/InvertedIndex.h"

namespace Lux {

  Searcher::Searcher(std::string storage_dir, DocumentDefinition &doc_def)
  : lhs_(new LuxHashStorage(storage_dir + "/" + LUX_META_DB, DB_RDONLY)),
    ds_(new LuxDocStorage(storage_dir, DB_RDONLY, doc_def)),
    si_(new InvertedIndex(storage_dir, DB_RDONLY, doc_def))
  {}

  Searcher::~Searcher()
  {}

  ResultSet Searcher::search(const char *query, Condition &cond)
  {
    return search(std::string(query), cond);
  }

  ResultSet Searcher::search(std::string query, Condition &cond)
  {
    ResultSet rs;
    Query *q = QueryParser::parse(query); 
    IndexResultSet irs = si_->search(q);
    
    if (irs.size() <= 0) {
      return rs;
    }

    // calculate where to get results from sorted ones
    rs.set_result_info(cond.num, cond.page, irs.size());

    // sort result by the way specified in cond.sort
    Sorter sorter(cond.sort, rs.get_base() + rs.get_num());
    sorter.sort(irs);

    for (int i = rs.get_base(); i < rs.get_base() + rs.get_num(); ++i) {
      Document doc = ds_->get(irs.at(i).doc_id);
      rs.add(Result(doc, irs.at(i).score));
    }

    delete q;  
    return rs;
  }

}
