#include "Engine.h"
#include "lux/Storage/InvertedIndex.h"
#include "lux/Document/document_definition.pb.h"

namespace Lux {

  Engine::Engine(Config::Document &doc_config)
  : ds(new LuxDocStorage(doc_config)),
    ai(new LuxAttrIndex(doc_config)),
    si(new InvertedIndex(doc_config)),
    im(new IDManager),
    is_opened(false)
  {}

  Engine::~Engine()
  {
    close();
  }

  bool Engine::open(std::string dir, db_flags_t oflags)
  {
    oflags_ = oflags;
    if (!si->open(dir, oflags)) {
      error_log("opening search index failed.");
      return false;
    }
    if (!ai->open(dir, oflags)) {
      error_log("opening attribute index failed.");
      return false;
    }
    if (!ds->open(dir, oflags)) {
      error_log("opening document storage failed.");
      return false;
    }
    if (!im->open(dir, oflags)) {
      error_log("opening id manager failed.");
      return false;
    }
    is_opened = true;
    return true;
  }

  bool Engine::close(void)
  {
    if (oflags_ ^ DB_RDONLY) {
      si->flush();
    }
    return true;
  }

}
