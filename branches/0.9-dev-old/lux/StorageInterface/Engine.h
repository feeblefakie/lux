#ifndef LUX_ENGINE_H
#define LUX_ENGINE_H

#include "lux/lux.h"
#include "lux/Storage/DocStorage.h"
#include "lux/Storage/AttrIndex.h"
#include "lux/Storage/IDManager.h"

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  class SearchIndex;
  //class Config::Document;

  class Engine {

  public:
    Engine(Config::Document &doc_config);
    ~Engine();
    bool open(std::string dir, db_flags_t oflags);
    bool close(void);

#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<LuxDocStorage> ds;
    std::tr1::shared_ptr<LuxAttrIndex> ai;
    std::tr1::shared_ptr<SearchIndex> si;
    std::tr1::shared_ptr<IDManager> im;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<LuxDocStorage> ds;
    boost::shared_ptr<LuxAttrIndex> ai;
    boost::shared_ptr<SearchIndex> si;
    boost::shared_ptr<IDManager> im;
#endif

    bool is_opened;
    db_flags_t oflags_;
  };
}

#endif
