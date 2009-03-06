#ifndef LUX_INDEXER_H
#define LUX_INDEXER_H

#include "lux/lux.h"
#include "Engine.h"

namespace Lux {

  class Document;

  class Indexer {

  public:
    Indexer(Engine &engine);
    ~Indexer(); 
    void set_index_buffer_threshold(uint32_t mega_bytes);
    void add(Document &doc);
    void add(Document *doc);

  private:
    Engine engine_;
    uint32_t index_buffer_threshold_;
  };
}

#endif
