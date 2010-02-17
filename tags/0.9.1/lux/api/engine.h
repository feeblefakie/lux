#ifndef LUX_API_ENGINE_H
#define LUX_API_ENGINE_H

#include "lux/lux.h"
#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  class SearchIndex;
  struct EngineImpl;
  namespace Config { class Document; }

  class Engine {

  public:
    Engine(Config::Document &doc_config);
    ~Engine();
    bool open(std::string dir, db_flags_t oflags);
    bool close(void);
    bool is_opened;
    db_flags_t oflags_;

#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<EngineImpl> pimpl_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<EngineImpl> pimpl_;
#endif
  };
}

#endif
