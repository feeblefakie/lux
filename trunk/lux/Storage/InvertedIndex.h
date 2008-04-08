#ifndef LUX_INVERTED_INDEX_H
#define LUX_INVERTED_INDEX_H

#include "lux.h"
#include "SearchIndex.h"
#include "InvertedIndexCore.h"
#include "Document/DocumentDefinition.h"
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
    InvertedIndex(std::string storage_dir, db_flags_t open_params,
                  DocumentDefinition &doc_def);
    virtual ~InvertedIndex();
    virtual void add(const Document *doc);
    virtual void add(const Document &doc);
    virtual void flush(void);
    virtual uint_t get_buffer_size(void);
    virtual void print_buffer(void);
    virtual IndexResultSet search(Query *query);
    virtual IndexResultSet search_term(TermQuery *qterm);
    virtual IndexResultSet search_phrase(PhraseQuery *qphrase);
    virtual IndexResultSet search_boolean(BooleanQuery *qboolean);

  private:
    scoped_ptr<LuxLexer> lexer_;
    IndexEngineSet index_engine_set_;
    scoped_ptr<IndexBufferSet> index_buffer_set_;
    scoped_ptr<IndexDataSet> index_data_set_;

    PostingList get_term_plist_(TermQuery *qterm);
    PostingList get_phrase_plist_(PhraseQuery *qphrase);
    IndexResultSet plist_to_rs_(PostingList &plist);

  };
}

#endif
