#ifndef LUX_API_DOCUMENTCONFIGPARSER_H
#define LUX_API_DOCUMENTCONFIGPARSER_H

#include "lux/document/document_definition.pb.h"

#include <string>

namespace Lux {

  /**
   * parser for a document configuration
   */
  class DocumentConfigParser {
  public:
    /**
     * parse the configuration
     * @param service service name
     * @param doc_config reference to Config::Document
     */
    static bool parse(std::string service, Config::Document &doc_config);
  private:
    static std::string default_config_dir_;

    static bool is_readable(const char *file);
  };
}

#endif
