#include "engine.h"
#include "engine-impl.h"
#include "lux/document/document_definition.pb.h"

namespace Lux {

  Engine::Engine(Config::Document &doc_config)
  : pimpl_(new EngineImpl(doc_config)),
    is_opened(false)
  {}

  Engine::~Engine()
  {
    close();
  }

  bool Engine::open(std::string dir, db_flags_t oflags)
  {
    oflags_ = oflags;
    if (!pimpl_->si->open(dir, oflags)) {
      error_log("opening search index failed.");
      return false;
    }
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
    is_opened = true;
    return true;
  }

  bool Engine::close(void)
  {
    if (oflags_ ^ DB_RDONLY) {
      pimpl_->si->flush();
    }
    return true;
  }

}
