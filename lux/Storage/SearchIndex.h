#ifndef LUX_SEARCHINDEX_H
#define LUX_SEARCHINDEX_H

#include "lux/lux.h"
#include "IndexResult.h"

namespace Lux {

  class Document;
  class Query;
  class TermQuery;
  class PhraseQuery;
  class BooleanQuery;

  class SearchIndex {

  public:
    virtual ~SearchIndex() {};
    virtual bool open(std::string storage_dir, db_flags_t open_params) = 0;
    virtual bool close(void) = 0;
    virtual void add(const Document *doc) = 0;
    virtual void add(const Document &doc) = 0;
    virtual void flush(void) = 0;
    virtual uint32_t get_buffer_size(void) = 0;
    virtual void print_buffer(void) {};
    virtual void set_sys_cond(sys_cond_t sys_cond) = 0;
    virtual IndexResultSet search(Query *query) = 0;
    virtual IndexResultSet search_term(TermQuery *qterm) = 0;
    virtual IndexResultSet search_phrase(PhraseQuery *qphrase) = 0;
    virtual IndexResultSet search_boolean(BooleanQuery *bquery) = 0;
  };
}

#endif
