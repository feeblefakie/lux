#include "lux/scoped_ptr.h"
#include "lux/index/in_memory_index.h"
#include "lux/index/doc_storage.h"
#include "lux/index/attr_index.h"
#include "lux/index/id_manager.h"

namespace Lux {

  /**
   * implementation of database engine interface
   */
  class BatchIndexerImpl {
  public:
    /**
     * constructor
     * @param doc_config reference to Confg::Document
     */
    explicit BatchIndexerImpl(Config::Document &doc_config)
    : mi(new TMP::InMemoryIndex(doc_config)),
      ds(new LuxDocStorage(doc_config)),
      ai(new LuxAttrIndex(doc_config)),
      im(new IDManager)
    {}
    ~BatchIndexerImpl()
    {
      delete mi;
      delete ds;
      delete ai;
      delete im;
    }
    /**
     * storage for in-memory index
     */
    TMP::InMemoryIndex *mi;
    /**
     * storage for display documents
     */
    LuxDocStorage *ds;
    /**
     * storage for attribute index
     */
    LuxAttrIndex *ai;
    /**
     * storage for ID management
     */
    IDManager *im;
  };

}
