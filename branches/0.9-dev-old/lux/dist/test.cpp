#include "manager.h"
#include <iostream>

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " service [port]" << std::endl;
    exit(1);
  }
  int port;
  if (argc >= 3) {
    port = atoi(argv[2]); 
  }
  Lux::Dist::Manager manager(argv[1], port, port+1);

  if (!manager.listen()) {
    std::cerr << "manager.listen() failed" << std::endl;
    exit(1);
  }

  manager.start_servers(); // loop
  std::cout << "end of main" << std::endl;

  return 0;
}
