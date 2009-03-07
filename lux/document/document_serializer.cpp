#include "document_serializer.h"
#include "lux/util.h"

namespace Lux {

  DocumentSerializer::DocumentSerializer(Config::Document &dd)
  {
    for (int i = 0; i < dd.field_size(); i++) {
      const Lux::Config::Field &field = dd.field(i);
      if (!field.display()) { continue; } 
      disp_field_map_.insert(make_pair(field.name(), "displayed"));
    }
  }

  /* 
   * serialized documet format:
   * "ID:{id}{fkey}:{fval}...."
   */ 
  // serialize
  std::string DocumentSerializer::pack(const Document &doc, bool disp_only)
  {
    std::string data = serialize_field(EXTID, doc.get_eid());

    doc.init_iter();
    while (doc.has_next()) {
      const Lux::Field *f = doc.get_next();

      // check if this field is being displayed
      if (disp_only) {
        SMItr sm_itr = disp_field_map_.find(f->get_name());
        if (sm_itr == disp_field_map_.end()) { continue; }
      }

      data += serialize_field(f->get_name(), f->get_value());
    }
    // chop the last separator
    data.erase(data.size()-1);

    return data;
  }

  // deserialize
  Document DocumentSerializer::unpack(char *data)
  {
    // deserialize and put fields back to Document object
    char *id = data;
    id = strstr(id, FIELD_SEP);
    if (id == NULL) {
      error_log("no id found.");
      return Document(ERR_DOC_ID);
    }
    *id++ = '\0';

    // get fields
    char *fields = strstr(id, FIELDS_SEP);
    if (fields == NULL) {
      error_log("no fields found. returning empty document.");
      return Document(ERR_DOC_ID);
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
  }

}
