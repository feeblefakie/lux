#ifndef LUX_INVERTED_INDEX_H
#define LUX_INVERTED_INDEX_H

#include "lux/lux.h"
#include "lux/scoped_ptr.h"
#include "SearchIndex.h"
#include "InvertedIndexCore.h"
#include "Document/document_definition.pb.h"
#include "Lexer/Lexer.h"
#include <cstring>

namespace Lux {

  class Query;
  class TermQuery;
  class PhraseQuery;
  class BooleanQuery;

  /**
   * class InvertedIndex
   */
  class InvertedIndex : public SearchIndex {

  public:
    InvertedIndex(Config::Document &doc_config);
    virtual ~InvertedIndex();
    virtual bool open(std::string storage_dir, db_flags_t open_params);
    virtual bool close(void);
    virtual void add(const Document *doc);
    virtual void add(const Document &doc);
    virtual void flush(void);
    virtual uint32_t get_buffer_size(void);
    virtual void print_buffer(void);
    virtual void set_sys_cond(sys_cond_t sys_cond);
    virtual IndexResultSet search(Query *query);
    virtual IndexResultSet search_term(TermQuery *qterm);
    virtual IndexResultSet search_phrase(PhraseQuery *qphrase);
    virtual IndexResultSet search_boolean(BooleanQuery *qboolean);

  private:
    scoped_ptr<LuxLexer> lexer_;
    IndexEngineSet index_engine_set_;
    scoped_ptr<IndexBufferSet> index_buffer_set_;
    scoped_ptr<IndexDataSet> index_data_set_;
    Config::Document doc_config_;
    sys_cond_t sys_cond_;

    LuxDataUnit *get_term_unit_(TermQuery *qterm);
    Posting get_term_pos_(TermQuery *qterm);
    Posting get_phrase_pos_(PhraseQuery *qphrase);
    IndexResultSet pos_to_rs_(Posting &p);
    pos_list_t _intersect(LuxDataUnit *u1, LuxDataUnit *u2, int diff);
    pos_list_t _intersect(LuxDataUnit *u, pos_list_t &p, int diff);
    pos_list_t _intersect(pos_list_t &p, LuxDataUnit *u, int diff);

  };
}

#endif
