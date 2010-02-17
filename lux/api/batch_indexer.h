#ifndef LUX_API_BATCHINDEXER_H
#define LUX_API_BATCHINDEXER_H

#include "lux/lux.h"
#include <string>
#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif
#include <vector>

namespace Lux {

  class Document;
  class BatchIndexerImpl;
  namespace Config { class Document; }

  struct term_list {
    term_list(char *term_, size_t term_size_,
              char *list_, size_t list_size_, uint32_t idx_)
    : term(term_), term_size(term_size_),
      list(list_), list_size(list_size_), idx(idx_)
    {}  

    char *term;
    size_t term_size;
    char *list;
    size_t list_size;
    uint32_t idx;
  };

  class greater_term : public std::binary_function<term_list, term_list, bool>
  {
    public:
    result_type operator() (first_argument_type &a, second_argument_type &b) 
    {
      return (result_type)((strcmp(a.term, b.term) > 0) ? 1 : 0);
    }
  };

  typedef std::vector<term_list> TLA;
  typedef TLA::iterator TLAITR;

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
    bool open(std::string dir);
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
    uint32_t flush_counter_;
    std::string dir_;

    bool flush(void);
    bool get_records(TLA &tla, int *fds, off_t *offs, uint32_t idx, uint32_t num);
  };
}

#endif
