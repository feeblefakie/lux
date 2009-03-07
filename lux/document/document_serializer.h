#ifndef LUX_DOCUMENTSERIALIZER_H
#define LUX_DOCUMENTSERIALIZER_H

#include "lux/lux.h"
#include "document.h"
#include "document_definition.pb.h"
#include <string>
#include <map>

#define EXTID "_ID"
#define FIELD_SEP ":"
#define FIELDS_SEP ""

namespace Lux {

  typedef std::map< std::string, std::string > StrMap;
  typedef StrMap::iterator SMItr;

  class DocumentSerializer {
  public:
    DocumentSerializer(void) {}
    DocumentSerializer(Config::Document &dd);
    ~DocumentSerializer(void) {}
    std::string pack(const Document &doc, bool disp_only = false);
    Document unpack(char *data);

  private:
    StrMap disp_field_map_;

    std::string serialize_field(std::string name, std::string val)
    {
      return name + FIELD_SEP + val + FIELDS_SEP;
    }
  };

}

#endif
