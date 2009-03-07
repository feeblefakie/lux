#include "document_config_parser.h"
#include "lux/util.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>

namespace Lux {

  std::string DocumentConfigParser::default_config_dir_ = "/etc/lux/";

  bool DocumentConfigParser::parse(std::string service,
                                   Config::Document &doc_config)
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string config_file = default_config_dir_ + service + ".document.conf";
    std::fstream fin(config_file.c_str(), std::ios::in); 
    google::protobuf::io::IstreamInputStream input(&fin);
    google::protobuf::TextFormat::Parser parser;
    if (!parser.Parse(&input, &doc_config)) {
      error_log("failed to parse.");
      return false;
    }
    return true;
  }

}
