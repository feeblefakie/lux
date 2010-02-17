#include "lux/index/inverted_index.h"
#include "lux/index/doc_storage.h"
#include "lux/index/attr_index.h"
#include "lux/index/id_manager.h"

namespace Lux {

  /**
   * implementation of database engine interface
   */
  struct EngineImpl {
    /**
     * constructor
     * @param doc_config reference to Confg::Document
     */
    explicit EngineImpl(Config::Document &doc_config)
    : ds(new LuxDocStorage(doc_config)),
      ai(new LuxAttrIndex(doc_config)),
      si(new InvertedIndex(doc_config)),
      im(new IDManager)
    {}
    /**
     * storage for display documents
     */
    LuxDocStorage *ds;
    /**
     * storage for attribute index
     */
    LuxAttrIndex *ai;
    /**
     * storage for search index
     */
    SearchIndex *si;
    /**
     * storage for ID management
     */
    IDManager *im;
  };

}
