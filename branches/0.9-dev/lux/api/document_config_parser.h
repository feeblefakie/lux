#ifndef LUX_API_DOCUMENTCONFIGPARSER_H
#define LUX_API_DOCUMENTCONFIGPARSER_H

#include "lux/document/document_definition.pb.h"

#include <string>

namespace Lux {

  class DocumentConfigParser {
  public:
    static bool parse(std::string service, Config::Document &doc_config);
  private:
    static std::string default_config_dir_;
  };
}

#endif
