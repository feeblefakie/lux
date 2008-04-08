#ifndef LUX_INDEXER_H
#define LUX_INDEXER_H

#include "lux/lux.h"
#include "lux/scoped_ptr.h"
#include "lux/Document/DocumentDefinition.h"
#include "lux/Storage/DocStorage.h"
#include "lux/StorageEngine/StorageEngine.h"

#define INTERNALID "__INTERNALID__"

// 64MByte in default
#define INDEX_BUFFER_THRESHOLD 64*1024*104

namespace Lux {

  class Document;
  class SearchIndex;

  typedef enum {
    ID_REGISTERED = 1,
    ID_NEW = 2,
  } id_status_t;

  class Indexer {

  public:
    Indexer(std::string storage_dir, DocumentDefinition &doc_def);
    ~Indexer(); 
    void add(Document &doc);

  private:
    DocumentDefinition doc_def_;
    id_status_t ext_to_int(std::string e, doc_id_t *i);
    scoped_ptr<LuxHashStorage> lhs_;
    doc_id_t curr_int_doc_id_;
    LuxDataUnit id_key_;

    scoped_ptr<LuxDocStorage> ds_;
    scoped_ptr<SearchIndex> si_;
  };
}

#endif
