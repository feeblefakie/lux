#include "Indexer.h"
#include "lux/Document/Document.h"
#include "lux/Storage/InvertedIndex.h"

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

  void Indexer::add(Document &doc)
  {
    id_status_t status = engine_.im->set_id(doc);
    if (status == FAILED) {
      // [TODO] do something
      return;
    } else if (status == REGISTERED) {
      // [TODO] update or do something
      return;
    } else {
      engine_.ds->add(doc);
      engine_.si->add(doc);
      engine_.ai->add(doc);
    }

    if (engine_.si->get_buffer_size() > index_buffer_threshold_) {
      engine_.si->flush();
    }
  }

  void Indexer::add(Document *doc)
  {
    add(*doc);
  }
}
