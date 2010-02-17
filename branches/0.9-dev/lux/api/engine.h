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

  /**
   * database engine interface
   */
  class Engine {

  public:
    /**
     * constructor
     * @param doc_config Reference to Config::Document
     */
    Engine(Config::Document &doc_config);
    /**
     * destructor
     */
    ~Engine();
    /**
     * open a database
     * @param dir directory which the database exists
     * @param oflags open flag
     */
    bool open(std::string dir, db_flags_t oflags);
    /**
     * close a database
     */
    bool close(void);
    /**
     * flag if the database is open or not
     */
    bool is_opened;
    /**
     * open flag
     */
    db_flags_t oflags_;

#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<EngineImpl> pimpl_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<EngineImpl> pimpl_;
#endif
  };
}

#endif
