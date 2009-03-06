#include "daemon_manager.h"
#include "index_server.h"
#include "server_config_helper.h"
#include "lux/search.h"
#include "lux/api/document_config_parser.h"
#include <iostream>
#include <stdexcept>

using namespace Lux::Dist;

int main(int argc, char *argv[])
{
  // TODO: using getopt
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " service db cluster_id server_id" << std::endl;
    exit(1);
  }
  char *service = argv[1];
  char *db = argv[2];
  int cluster_id = atoi(argv[3]);
  int server_id = atoi(argv[4]);

  try {
    ServerConfigHelper config_helper(service);

    Lux::Config::Cluster cluster = config_helper.get_cluster(cluster_id);
    Lux::Config::Server server = cluster.server(server_id);
    // [TODO] check if specified host is myself


    Lux::Config::Document doc_config;
    if (!Lux::DocumentConfigParser::parse(argv[1], doc_config)) {
      std::cerr << "parse failed." << std::endl;
      exit(1);
    }

    /*
    Lux::DocumentDefinition dd("blog");
    static Lux::FieldDefinition title_fd("title");
    title_fd.set_indexed(true);
    title_fd.set_displayed(true);
    dd.add(title_fd);
    static Lux::FieldDefinition content_fd("content");
    content_fd.set_indexed(true);
    content_fd.set_displayed(true);
    dd.add(content_fd);
    static Lux::FieldDefinition url_fd("url");
    url_fd.set_displayed(true);
    dd.add(url_fd);
    static Lux::FieldDefinition createdat_fd("created_at");
    createdat_fd.set_displayed(true);
    createdat_fd.set_attr_indexed(true, Lux::ATTR_STR, 15);
    dd.add(createdat_fd);
    */

    Lux::Engine engine(doc_config);
    if (!engine.open(db, Lux::DB_CREAT)) {
      throw std::runtime_error("engine.open() failed");
    }

    DaemonManager<IndexServer> manager(server.port(), 
                                       server.num_default(),
                                       server.num_max());
    manager.add_managee_arg((void *) &engine);

    if (!manager.listen()) {
      throw std::runtime_error("manager.listen() failed");
    }
    manager.start_servers(); // loop

  } catch (std::runtime_error &e) {
    error_log(e.what());
    exit(1);
  }

  return 0;
}
