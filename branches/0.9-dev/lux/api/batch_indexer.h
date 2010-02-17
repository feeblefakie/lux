#ifndef LUX_API_BATCHINDEXER_H
#define LUX_API_BATCHINDEXER_H

#include "lux/lux.h"
#include <string>
#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  class Document;
  class BatchIndexerImpl;
  namespace Config { class Document; }

  /**
   * indexer interface
   */
  class BatchIndexer {

  public:
    /**
     * constructor
     * @param engine database engine object
     */
    BatchIndexer(Config::Document &doc_config);
    /**
     * destructor
     */
    ~BatchIndexer(); 
    /**
     * open a BatchIndexer
     */
    bool open(std::string dir, db_flags_t oflags);
    /**
     * close a BatchIndexer
     */
    bool close(void);
    /**
     * set index buffer size threshold
     * @param mege_bytes It can be specified size in mega bytes
     * @note It is a hint to the engine, so it can use more memory than specified
     */
    void set_index_buffer_threshold(uint32_t mega_bytes);
    /**
     * add a document
     * @param reference to Document
     */
    bool add(Document &doc);
    /**
     * add a document
     * @param pointer to Document
     */
    bool add(Document *doc);
    /**
     * merge the flushed sub-indexes
     */
    bool merge(void);

  private:
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<BatchIndexerImpl> pimpl_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<BatchIndexerImpl> pimpl_;
#endif
    uint32_t index_buffer_threshold_;

  };
}

#endif
