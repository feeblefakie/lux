#include "lux/index/inverted_index.h"
#include "lux/index/doc_storage.h"
#include "lux/index/attr_index.h"
#include "lux/index/id_manager.h"

namespace Lux {

  struct EngineImpl {
    explicit EngineImpl(Config::Document &doc_config)
    : ds(new LuxDocStorage(doc_config)),
      ai(new LuxAttrIndex(doc_config)),
      si(new InvertedIndex(doc_config)),
      im(new IDManager)
    {}
    LuxDocStorage *ds;
    LuxAttrIndex *ai;
    SearchIndex *si;
    IDManager *im;
  };

}
