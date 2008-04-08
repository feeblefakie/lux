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

  class DocStorageHash {
      
  public:
    DocStorageHash(std::string storage_dir, db_flags_t open_params,
                   DocumentDefinition &doc_def)
    : ldhs_(new LuxDHashStorage(storage_dir + "/" + LUX_DOC_DB, open_params))
    {}

    ~DocStorageHash() {}

    void add(const Document *doc)
    {
      add(*doc);
    }

    void add(const Document &doc)
    {
      doc_id_t internal_id = doc.get_internal_id();  

      //std::cout << "DocumentStorage : internal id : " << internal_id << std::endl;

      std::string fields = serialize_field(std::string(EXTID), 
                                           (std::string)doc.get_external_id());

      doc.init_iter();
      while (doc.has_next()) {
        const Lux::Field *f = doc.get_next();
        //std::cout << f->get_name() << " : " << f->get_value() << std::endl;

        // see field definition and decide what to do.
        if (f->def()->is_displayed()) {
          fields += serialize_field(f->get_name(), f->get_value());
        }
      }
      // chop the last separator
      fields.erase(fields.size()-1);
      
      //std::cout << fields << std::endl;

      LuxDataUnit doc_key(&internal_id, sizeof(doc_id_t));
      LuxDataUnit doc_val_serialized(const_cast<char *>(fields.c_str()), fields.size());
     
      // compress option should be provided by storage engine
      if (!ldhs_->put(doc_key, doc_val_serialized)) {
        std::cerr << "[error] putting in doc storage failed." << std::endl;
      }
    }

    Document get(doc_id_t internal_id)
    {
      // deserialize and put fields back to Document object
      LuxDataUnit doc_key(&internal_id, sizeof(doc_id_t));
      LuxDataUnit doc_val_serialized;
      if (!ldhs_->get(doc_key, doc_val_serialized)) {
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
      for (char *field = strtok(fields, FIELDS_SEP); 
         field; 
         field = strtok(NULL, FIELDS_SEP)) {

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
    scoped_ptr<LuxDHashStorage> ldhs_;

    //std::string serialize_field(std::string &name, std::string &val)
    std::string serialize_field(std::string name, std::string val)
    {
      return name + FIELD_SEP + val + FIELDS_SEP;
    }
  };
}

#endif
