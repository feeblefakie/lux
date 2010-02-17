#ifndef LUX_API_INDEXER_H
#define LUX_API_INDEXER_H

#include "lux/lux.h"
#include "engine.h"

namespace Lux {

  class Document;

  /**
   * indexer interface
   */
  class Indexer {

  public:
    /**
     * constructor
     * @param engine database engine object
     */
    Indexer(Engine &engine);
    /**
     * destructor
     */
    ~Indexer(); 
    /**
     * set index buffer size threshold
     * @param mege_bytes It can be specified size in mega bytes
     * @note It is a hint to the engine, so it can use more memory than specified
     */
    void set_index_buffer_threshold(uint32_t mega_bytes);
    /**
     * add a document to the engine
     * @param reference to Document
     */
    bool add(Document &doc);
    /**
     * add a document to the engine
     * @param pointer to Document
     */
    bool add(Document *doc);
    /**
     * delete a document from the engine
     * @param reference to Document
     * @note deletion is only logically for now
     */
    bool del(Document &doc);
    /**
     * delete a document from the engine
     * @param pointer to Document
     * @note deletion is only logically for now
     */
    bool del(Document *doc);

  private:
    Engine engine_;
    uint32_t index_buffer_threshold_;
  };
}

#endif
