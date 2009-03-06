#include "server_config_helper.h"
#include "lux/lux.h"
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>
#include <string>
#include <stdexcept>

namespace Lux {
namespace Dist {

  std::string ServerConfigHelper::default_config_dir_ = "/etc/lux/";

  ServerConfigHelper::ServerConfigHelper(std::string service)
  : config_(new Lux::Config::ServerConfig)
  {
    std::string config_file = default_config_dir_ + service + ".server.conf";
    std::fstream fin(config_file.c_str(), std::ios::in); 
    google::protobuf::io::IstreamInputStream input(&fin);
    google::protobuf::TextFormat::Parser parser;
    if (!parser.Parse(&input, config_)) {
      throw std::runtime_error("failed to parse server configuration.");
    }  
  }

  ServerConfigHelper::~ServerConfigHelper(void)
  {}

  Lux::Config::Dispatchers
  ServerConfigHelper::get_dispatchers(void)
  {
    return config_->dispatchers();
  }

  Lux::Config::Dispatcher
  ServerConfigHelper::get_dispatcher(int dispatcher_id)
  {
    return config_->dispatchers().dispatcher(dispatcher_id);
  }

  Lux::Config::Cluster
  ServerConfigHelper::get_cluster(int cluster_id)
  {
    Lux::Config::Clusters clusters = config_->clusters();
    if (cluster_id < 0 &&
        cluster_id >= clusters.cluster_size()) {
      throw std::runtime_error("non-existing cluster_id.");
    }
    return clusters.cluster(cluster_id);
  }

}
}
