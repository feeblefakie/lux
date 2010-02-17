#include "batch_indexer.h"
#include "batch_indexer-impl.h"
#include "lux/document/document.h"
#include "lux/index/in_memory_index.h"

namespace Lux {

  BatchIndexer::BatchIndexer(Config::Document &doc_config)
  : index_buffer_threshold_(64*1024*1024),
    pimpl_(new BatchIndexerImpl(doc_config))
  {}

  BatchIndexer::~BatchIndexer()
  {}

  bool BatchIndexer::open(std::string dir, db_flags_t oflags)
  {
    if (!pimpl_->ai->open(dir, oflags)) {
      error_log("opening attribute index failed.");
      return false;
    }
    if (!pimpl_->ds->open(dir, oflags)) {
      error_log("opening document storage failed.");
      return false;
    }
    if (!pimpl_->im->open(dir, oflags)) {
      error_log("opening id manager failed.");
      return false;
    }
    return true;
  }

  bool BatchIndexer::close(void)
  {}

  void BatchIndexer::set_index_buffer_threshold(uint32_t mega_bytes)
  {
    // [TODO] should be multiplied by 2 for estimate
    index_buffer_threshold_ = mega_bytes * 1024 * 1024;
  }

  bool BatchIndexer::add(Document &doc)
  {
    id_status_t status = pimpl_->im->set_id(doc);
    if (status == FAILED) {
      return false;
    } else if (status == REGISTERED) {
      // [TODO] update or do something
      return true;
    } else {
      if (!pimpl_->mi->add(doc)) {
        error_log("adding document into in-memory index failed.");
        return false;
      }
      if (!pimpl_->ds->add(doc)) {
        error_log("adding document into document storage failed.");
        return false;
      }
      if (!pimpl_->ai->add(doc)) {
        error_log("adding document into attribute index failed.");
        return false;
      }
    }

    if (pimpl_->mi->get_buffer_size() > index_buffer_threshold_) {
      // flush
      //
      //
    }
    return true;
  }

  bool BatchIndexer::add(Document *doc)
  {
    return add(*doc);
  }

  bool BatchIndexer::merge(void)
  {
    return true;
  }

}
