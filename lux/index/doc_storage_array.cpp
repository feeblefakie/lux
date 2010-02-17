#include "doc_storage_array.h"
#include "lux/document/document_definition.pb.h"
#include "lux/document/document.h"

namespace Lux {

  DocStorageArray::DocStorageArray(Config::Document &doc_config)
  : doc_storage_(new LuxArrayStorage),
    doc_serializer_(doc_config)
  {}

  bool DocStorageArray::open(std::string storage_dir, db_flags_t open_params)
  {
    doc_storage_->set_index_type(IO::NONCLUSTER);
    std::string db_name = storage_dir + "/" + LUX_DOC_DB;
    if (!doc_storage_->open(db_name.c_str(), open_params)) {
      error_log("DocStorage open failed");
      return false;
    }
    return true;
  }

  bool DocStorageArray::close(void)
  {
    if (!doc_storage_->close()) {
      return false;
    }
    return true;
  }

  bool DocStorageArray::add(const Document *doc)
  {
    return add(*doc);
  }

  bool DocStorageArray::add(const Document &doc)
  {
    LuxDataUnit doc_serialized;
    // serialize only display fields
    std::string fields = doc_serializer_.pack(doc, true);

#ifdef USE_ZLIB
    if (!_compress(fields, doc_serialized)) { return false; }
#else
    doc_serialized.set_data(const_cast<char *>(fields.c_str()));
    doc_serialized.set_size(fields.size());
#endif
    if (!doc_storage_->put(doc.get_id(), doc_serialized)) {
      error_log("putting in doc storage failed.");
      return false;
    }
    return true;
  }

  Document DocStorageArray::get(doc_id_t internal_id)
  {
    LuxDataUnit doc_serialized;
    if (!doc_storage_->get(internal_id, doc_serialized)) {
      error_log("getting from doc storage failed.");
      return Document(ERR_DOC_ID);
    }

    char *data;
#ifdef USE_ZLIB
    std::string uncompressed;
    if (!_uncompress(doc_serialized, uncompressed)) {
      return Document(ERR_DOC_ID);
    }
    data = const_cast<char *>(uncompressed.c_str());
#else
    data = static_cast<char *>(doc_serialized.get_data());
#endif

    return doc_serializer_.unpack(data);
  }

  // [FIXME] should fix this redundant code. 
  std::string DocStorageArray::get_raw(doc_id_t internal_id)
  {
    LuxDataUnit doc_serialized;
    if (!doc_storage_->get(internal_id, doc_serialized)) {
      error_log("getting from doc storage failed.");
      return std::string();
    }

    char *data;
#ifdef USE_ZLIB
    std::string uncompressed;
    if (!_uncompress(doc_serialized, uncompressed)) {
      return std::string();
    }
    data = const_cast<char *>(uncompressed.c_str());
#else
    data = static_cast<char *>(doc_serialized.get_data());
#endif

    return std::string(data);
  }

#ifdef USE_ZLIB
  bool DocStorageArray::_compress(std::string &src, LuxDataUnit &dest)
  {
    char *data;
    uint32_t size;
    uint32_t comp_len;
    char *comp = compress((char *) src.c_str(), src.size(), &comp_len);
    if (comp == NULL ||
        comp_len + DOC_HDR_SIZE >= src.size()) {
      size = src.size() + sizeof(char);
      data = new char[size];
      data[0] = NONCOMPRESSED;
      strncpy(data + 1, src.c_str(), src.size());
    } else {
      size = comp_len + DOC_HDR_SIZE;
      data = new char[size];
      data[0] = COMPRESSED;
      uint32_t org_size = src.size();
      memcpy(data + 1, &org_size, sizeof(uint32_t));
      memcpy(data + DOC_HDR_SIZE, comp, comp_len);
    }
    if (comp != NULL) { delete [] comp; }
    dest.set_cleanup_needed(true);
    dest.set_data(data);
    dest.set_size(size);

    return true;
  }

  bool DocStorageArray::_uncompress(LuxDataUnit &src, std::string &dest)
  {
    char *data = (char *) src.get_data();
    if (data[0] == COMPRESSED) {
      uint32_t dest_size;
      memcpy(&dest_size, data + 1, sizeof(uint32_t));
      char *p = uncompress(data + DOC_HDR_SIZE,
                           src.get_size() - DOC_HDR_SIZE, &dest_size);
      if (p == NULL) { return false; }
      dest = p;
      delete [] p;
    } else {
      dest = data + 1;
    }
    return true;
  }
#endif
}
