#ifndef LUX_DIST_SERVERCONFIG_H
#define LUX_DIST_SERVERCONFIG_H

#include "server-config.pb.h"

namespace Lux {
namespace Dist {

  class ServerConfigHelper {
  public:
    ServerConfigHelper(std::string service);
    ~ServerConfigHelper(void);
    Lux::Config::Dispatchers get_dispatchers(void);
    Lux::Config::Dispatcher get_dispatcher(int dispatcher_id);
    Lux::Config::Cluster get_cluster(int cluster_id);
  private:
    Lux::Config::ServerConfig *config_;
    static std::string default_config_dir_;
  };

}
}

#endif
