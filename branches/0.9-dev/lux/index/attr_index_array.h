#ifndef LUX_INDEX_ATTRINDEXARRAY_H
#define LUX_INDEX_ATTRINDEXARRAY_H

#include "lux/lux.h"
#include "lux/storage/storage_engine.h"
#include "lux/document/document_definition.pb.h"
#include <string>
#include <set>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  class Document;

  template <class T>    
  struct less_attr_name : public std::binary_function <T, T, bool> {
    bool operator() (const T& t1, const T& t2) const {
      return t1.attr_name < t2.attr_name;
    }
  };

  /**
   * class AttrIndexEngine
   */
  struct AttrIndexEngine {
    AttrIndexEngine(const std::string &attr_name_,
        Config::Field::AttrIndexType attr_type_, uint32_t attr_size_);
    AttrIndexEngine(const std::string &attr_name_);
    bool open(const std::string file, db_flags_t open_params_);
    bool close(void);

    std::string attr_name;
    Config::Field::AttrIndexType attr_type;
    uint32_t attr_size;
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<LuxArrayStorage> engine;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<LuxBtreeStorage> engine;
#endif
  };

  typedef std::set< AttrIndexEngine,
          less_attr_name<AttrIndexEngine> > engine_set;
  typedef engine_set::iterator engine_set_itr;

  /**
   * class AttrIndexArray
   */
  class AttrIndexArray {
  public:
    AttrIndexArray(Config::Document &doc_config);
    ~AttrIndexArray() {}
    bool open(std::string storage_dir, db_flags_t open_params);
    bool close(void);
    bool add(const Document *doc);
    bool add(const Document &doc);
    bool get(AttrIndexEngine &aie);

  private:
    engine_set engine_set_;
    Config::Document doc_config_;
  };
}

#endif
