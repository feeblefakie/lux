#ifndef LUX_INDEX_IDMANAGER_H
#define LUX_INDEX_IDMANAGER_H

#include "lux/lux.h"
#include "lux/storage/storage_engine.h"
#include <string>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

#define IID "__IID__"

namespace Lux {

  class Document;

  class IDManager {
  public:
    IDManager(void);
    ~IDManager();
    bool open(std::string dir, db_flags_t oflags = DB_CREAT);
    bool close(void);
    id_status_t set_id(Document *doc);
    id_status_t set_id(Document &doc);

  private:
    doc_id_t curr_id_;
    LuxDataUnit id_key_;
    db_flags_t oflags_;
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<LuxBtreeStorage> manager_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<LuxBtreeStorage> manager_;
#endif
  };
}

#endif
