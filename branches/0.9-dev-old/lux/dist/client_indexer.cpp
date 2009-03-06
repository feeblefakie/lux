#include "client_manager.h"
#include "indexer.h"
#include "update-protocol.pb.h"
#include <string>
#include <boost/algorithm/string.hpp>

int main(void)
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // read server configuration start
  std::vector<Lux::Dist::server_inf_t> servers;
  Lux::Dist::server_inf_t server;
  server.ip = Lux::get_ip_by_str("localhost");
  server.port = 4306;
  servers.push_back(server);
  // read server configuration end

  Lux::Dist::ClientManager<Lux::Dist::Indexer,
                           Lux::Protocol::Updates> manager(servers);
  manager.start_clients();

  Lux::Protocol::Updates updates;
  Lux::Protocol::Update *update = updates.add_update();
  update->set_type(Lux::Protocol::Update::ADD);
  /*
  Lux::Protocol::Document *document = update->mutable_document();
  document->set_id("http://hoge.com");
  Lux::Protocol::Field *field = document->add_field();
  field->set_name("title");
  field->set_value("this is a title of a field.");
  */

  int i = 0;
  std::string line;
  while (getline(std::cin, line)) {
    std::vector<std::string> items;
    boost::split(items, line, boost::is_any_of("^X"));

    // invalid input
    if (items.size() != 6) { continue; }

    Lux::Protocol::Document *document = update->mutable_document();
    document->set_id(items[0]);
    Lux::Protocol::Field *field = document->add_field();
    field->set_name("url");
    field->set_value(items[3]);
    field->set_name("title");
    field->set_value(items[4]);
    field->set_name("content");
    field->set_value(items[5]);
    field->set_name("created_at");
    field->set_value(items[2]);

    if (i % 10 == 0) {
      manager.enqueue_message(updates);
      updates.clear_update();
    }
  }

  manager.finish_clients();

  std::cout << "end" << std::endl;

  return 0;
}
