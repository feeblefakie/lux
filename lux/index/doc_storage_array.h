#ifndef LUX_INDEX_DOCSTORAGEARRAY_H
#define LUX_INDEX_DOCSTORAGEARRAY_H

#include "lux/lux.h"
#include "lux/scoped_ptr.h"
#include "lux/storage/storage_engine.h"
#include "lux/document/document_serializer.h"
#include <string>
#include <map>

#define COMPRESSED 0x10
#define NONCOMPRESSED 0x00

#define DOC_HDR_SIZE sizeof(char) + sizeof(uint32_t)
// [TODO] making structure is easier

namespace Lux {

  class Document;
  namespace Config { class Document; }

  typedef std::map< std::string, std::string > StrMap;
  typedef StrMap::iterator SMIterator;

  class DocStorageArray {
  public:
    DocStorageArray(Config::Document &doc_config);
    ~DocStorageArray() {}
    bool open(std::string storage_dir, db_flags_t open_params);
    bool close(void);
    bool add(const Document *doc);
    bool add(const Document &doc);
    Document get(doc_id_t internal_id);
    std::string get_raw(doc_id_t internal_id);

  private:
    scoped_ptr<LuxArrayStorage> doc_storage_;
    StrMap disp_field_map_;
    DocumentSerializer doc_serializer_;

#ifdef USE_ZLIB
    bool _compress(std::string &src, LuxDataUnit &dest);
    bool _uncompress(LuxDataUnit &src, std::string &dest);
#endif
  };
}

#endif
