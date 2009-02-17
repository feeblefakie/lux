#include "daemon_manager.h"
#include "dispatcher.h"
#include "server_config_helper.h"
#include <iostream>

using namespace Lux::Dist;

int main(int argc, char *argv[])
{
  // TODO: using getopt
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " service dispatcher_id" << std::endl;
    exit(1);
  }
  char *service = argv[1]; 
  int dispatcher_id = atoi(argv[2]); 

  try {
    ServerConfigHelper config_helper(service);
    Lux::Config::Dispatcher dispatcher = 
        config_helper.get_dispatcher(dispatcher_id);

    // [TODO] check if specified host is myself

    Lux::Config::Cluster cluster = 
        config_helper.get_cluster(dispatcher.cluster_id());
    Lux::Config::Server server = dispatcher.server(); 

    DaemonManager<Dispatcher> manager(server.port(),
                                      server.num_default(),
                                      server.num_max());
    manager.add_managee_arg(&cluster);

    if (!manager.listen()) {
      throw std::runtime_error("manager.listen() failed");
    }

    manager.start_servers(); // loop

  } catch (std::runtime_error &e) {
    error_log(e.what());
    exit(1);
  } catch (...) {
    error_log("fatal error");
    exit(1);
  }

  return 0;
}
