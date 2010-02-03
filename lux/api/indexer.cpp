#include "indexer.h"
#include "engine-impl.h"
#include "lux/document/document.h"
#include "lux/index/inverted_index.h"

namespace Lux {

  Indexer::Indexer(Engine &engine)
  : index_buffer_threshold_(512*1024*1024),
    engine_(engine)
  {}

  Indexer::~Indexer()
  {}

  void Indexer::set_index_buffer_threshold(uint32_t mega_bytes)
  {
    index_buffer_threshold_ = mega_bytes * 1024 * 1024;
  }

  bool Indexer::add(Document &doc)
  {
    id_status_t status = engine_.pimpl_->im->set_id(doc);
    if (status == FAILED) {
      // [TODO] do something
      return false;
    } else if (status == REGISTERED) {
      // [TODO] update or do something
      return true;
    } else {
      if (!engine_.pimpl_->ds->add(doc)) {
        error_log("adding document into document storage failed.");
        return false;
      }
      if (!engine_.pimpl_->si->add(doc)) {
        error_log("adding document into search index failed.");
        return false;
      }
      if (!engine_.pimpl_->ai->add(doc)) {
        error_log("adding document into attribute index failed.");
        return false;
      }
    }

    if (engine_.pimpl_->si->get_buffer_size() > index_buffer_threshold_) {
      // [TODO] error handling
      engine_.pimpl_->si->flush();
    }
    return true;
  }

  bool Indexer::add(Document *doc)
  {
    return add(*doc);
  }

  bool Indexer::del(Document &doc)
  {
    // get_id to check if it's registered.
    // return unless it's registered
    doc_id_t doc_id;
    id_status_t status = engine_.pimpl_->im->set_id(doc, false);
    if (status == UNREGISTERED) {
      return true;
    } else {
      // MUST BE REGISTERED
      
      // these 1,2 must be done successfully
      // 1. update delete vector (to value 1) # also, update delete vector when adding. (to value 0)
      // 2. im->del 

      // these 3,4,5 can be failed. won't care if succeeded or not.
      // 3. ds->del (it saves some space for the releasing value. key(doc_id) is never used.)
      // 4. si->del (it's meaningless because don't have doc_id->terms relations, so nothing is done in the method.)
      // 5. ai->del (it's meaningless because it's clustered index. so nothing is done in the method.)
    }

    return true;
  }

  bool Indexer::del(Document *doc)
  {
    return del(*doc);
  }

}
