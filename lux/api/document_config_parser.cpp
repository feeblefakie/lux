#include "document_config_parser.h"
#include "lux/util.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

namespace Lux {

  std::string DocumentConfigParser::default_config_dir_ = "/etc/lux/";

  bool DocumentConfigParser::parse(std::string service,
                                   Config::Document &doc_config)
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string config_file = service + ".document.conf";
    std::vector<std::string> config_cands;
    config_cands.push_back(config_file); // current directory
    char *home_dir = getenv("HOME");
    if (home_dir != NULL) {
      config_cands.push_back(std::string(home_dir) + "/.lux/" + config_file); // HOME
    }
    config_cands.push_back(default_config_dir_ + config_file); // /etc/lux

    std::string config_file_full;
    std::vector<std::string>::iterator itr_end = config_cands.end();
    for (std::vector<std::string>::iterator itr = config_cands.begin();
         itr != itr_end; ++itr) {

      if (is_readable(itr->c_str())) {
        config_file_full = *itr;
        break;
      }
    }

    if (config_file_full.empty()) {
      return false; // not found or not readable
    }

    std::fstream fin(config_file_full.c_str(), std::ios::in); 
    google::protobuf::io::IstreamInputStream input(&fin);
    google::protobuf::TextFormat::Parser parser;
    if (!parser.Parse(&input, &doc_config)) {
      error_log("failed to parse.");
      return false;
    }
    return true;
  }

  bool DocumentConfigParser::is_readable(const char *file)
  {
    struct stat st; 
    if (stat(file, &st) != -1 && st.st_mode & S_IRUSR) {
      return true;
    }
    return false;
  }
}
