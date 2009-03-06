#ifndef LUX_DIST_INDEXCLIENT_H
#define LUX_DIST_INDEXCLIENT_H

#include "common.h"

namespace Lux {
namespace Dist {

  class Indexer {
  public:
    Indexer(void *p);
    ~Indexer(void);
    void process(void);

  private:
    client_thread_inf_t *client_;
    server_inf_t *server_;
  };

}
}

#endif
