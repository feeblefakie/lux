#ifndef LUX_DOCSTORAGEHASH_H
#define LUX_DOCSTORAGEHASH_H

#include "lux/lux.h"
#include "lux/scoped_ptr.h"
#include "lux/StorageEngine/StorageEngine.h"
#include "lux/Document/DocumentDefinition.h"
#include "lux/Document/Document.h"
#include <iostream>
#include <string>
#include <map>

#define EXTID "_ID"
#define FIELD_SEP ":"
#define FIELDS_SEP ""

namespace Lux {

  typedef std::map< std::string, std::string > StrMap;
  typedef StrMap::iterator SMIterator;

  class DocStorageHash {
      
  public:
    DocStorageHash(std::string storage_dir, db_flags_t open_params,
                   DocumentDefinition &doc_def)
    : doc_storage_(new LuxArrayStorage)
    {
      doc_storage_->set_index_type(IO::NONCLUSTER);
      std::string db_name = storage_dir + "/" + LUX_DOC_DB;
      if (!doc_storage_->open(db_name.c_str(), open_params)) {
        std::cerr << "DocStorage open failed" << std::endl;
      }
      doc_def.init_iter();
      while (doc_def.has_next()) {
        FieldDefinition fd = doc_def.get_next();
        if (!fd.is_displayed()) { continue; }

        std::string field_name = fd.get_field_name();
        disp_field_map_.insert(make_pair(field_name, "displayed"));
      }
    }

    ~DocStorageHash() {}

    void add(const Document *doc)
    {
      add(*doc);
    }

    void add(const Document &doc)
    {
      doc_id_t internal_id = doc.get_internal_id();  
      std::string fields = serialize_field(std::string(EXTID), 
                                           (std::string)doc.get_external_id());

      doc.init_iter();
      while (doc.has_next()) {
        const Lux::Field *f = doc.get_next();

        // check if this field is being displayed
        SMIterator sm_itr = disp_field_map_.find(f->get_name());
        if (sm_itr == disp_field_map_.end()) { continue; }

        fields += serialize_field(f->get_name(), f->get_value());
      }
      // chop the last separator
      fields.erase(fields.size()-1);
      
      //LuxDataUnit doc_key(&internal_id, sizeof(doc_id_t));
      LuxDataUnit doc_val_serialized(const_cast<char *>(fields.c_str()), fields.size());
     
      // compress option should be provided by storage engine
      if (!doc_storage_->put(internal_id, doc_val_serialized)) {
        std::cerr << "[error] putting in doc storage failed." << std::endl;
      }
    }

    Document get(doc_id_t internal_id)
    {
      // deserialize and put fields back to Document object
      //LuxDataUnit doc_key(&internal_id, sizeof(doc_id_t));
      LuxDataUnit doc_val_serialized;
      if (!doc_storage_->get(internal_id, doc_val_serialized)) {
        std::cerr << "[error] getting from doc storage failed." << std::endl;
      }

      /* 
       * serialized documet format:
       * "ID:{id}{fkey}:{fval}...."
       */ 
      // get id
      char *id = static_cast<char *>(doc_val_serialized.get_data());
      id = strstr(id, FIELD_SEP);
      if (id == NULL) {
        std::cerr << "[error] no id found." << std::endl;
        // fatal error
      }
      *id++ = '\0';

      // get fields
      char *fields = strstr(id, FIELDS_SEP);
      if (fields == NULL) {
        std::cerr << "[error] no fields found. returning empty document." << std::endl;
        Document doc(id);
        return doc;
      }
      *fields++ = '\0';

      // construct Document object
      Document doc(id);
      char *saveptr;
      for (char *field = strtok_r(fields, FIELDS_SEP, &saveptr); 
           field; 
           field = strtok_r(NULL, FIELDS_SEP, &saveptr)) {

        char *value = strstr(field, FIELD_SEP);
        *value++ = '\0';
        doc.add(Lux::Field::create(field, value));
      }
      return doc;

      // 今の段階だと、ここでdocが解放されてdocのデストラクタが呼ばれて、
      // 内部のFieldもdeleteされてしまう。
      // 何らかの解決策が必要
      // Documentのコピーコンストラクタで、
      // Fieldをコピー？
      // Vectorをコピーしたときの挙動を調べるべき(内部にポインタを持ってる場合はどうするか？　　)
    }

  private:
    //scoped_ptr<LuxDHashStorage> ldhs_;
    scoped_ptr<LuxArrayStorage> doc_storage_;
    StrMap disp_field_map_;

    //std::string serialize_field(std::string &name, std::string &val)
    std::string serialize_field(std::string name, std::string val)
    {
      return name + FIELD_SEP + val + FIELDS_SEP;
    }
  };
}

#endif
