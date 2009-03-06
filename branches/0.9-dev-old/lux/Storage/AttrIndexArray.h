#ifndef LUX_ATTRINDEXARRAY_H
#define LUX_ATTRINDEXARRAY_H

#include "lux/lux.h"
#include "lux/StorageEngine/StorageEngine.h"
#include "lux/Document/document_definition.pb.h"
#include "lux/Document/Document.h"
#include <string>
#include <set>
#include <functional>
#include <algorithm>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

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
        Config::Field::AttrIndexType attr_type_, uint32_t attr_size_)
    : attr_name(attr_name_),
      attr_type(attr_type_),
      attr_size(attr_size_),
      engine(new LuxArrayStorage)
    {}

    // for comparison
    AttrIndexEngine(const std::string &attr_name_)
    : attr_name(attr_name_)
    {}

    bool open(const std::string file, db_flags_t open_params_)
    {
      engine->set_index_type(IO::CLUSTER, attr_size);
      return engine->open(file.c_str(), open_params_);
    }

    bool close(void)
    {
      return engine->close();
    }

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
    AttrIndexArray(Config::Document &doc_config)
    : doc_config_(doc_config)
    {}

    ~AttrIndexArray() {}

    bool open(std::string storage_dir, db_flags_t open_params)
    {
      for (int i = 0; i < doc_config_.field_size(); i++) {
        const Lux::Config::Field &field = doc_config_.field(i);

        if (!field.attr_index().indexing()) { continue; }
        AttrIndexEngine aie(field.name(),
            field.attr_index().type(), field.attr_index().size());
        std::string db_name = storage_dir + "/" + field.name();

        if (!aie.open(db_name, open_params)) {
          error_log("opening attribute index failed.");
          continue;
        }
        engine_set_.insert(aie);
      }
      return true;
    }

    bool close(void)
    {
      return true;
    }

    bool add(const Document *doc)
    {
      return add(*doc);
    }

    bool add(const Document &doc)
    {
      doc.init_iter();
      while (doc.has_next()) {
        const Lux::Field *f = doc.get_next();

        // check if this field is being displayed
        engine_set_itr itr = 
          engine_set_.find(AttrIndexEngine(f->get_name()));
        if (itr == engine_set_.end()) { continue; }

        char data[itr->attr_size];
        memset(data, 0, itr->attr_size);
        const char *value = f->get_value().c_str();
        switch (itr->attr_type) {
          case Config::Field::STRING:
            memcpy(data, value, itr->attr_size);
            break;
          case Config::Field::INT:
            int num = atoi(value);
            memcpy(data, &num, itr->attr_size);
            break;
        }
        LuxDataUnit unit(data, itr->attr_size);
        if (!itr->engine->put(doc.get_id(), unit)) {
          error_log("putting attribute index failed");
        }
      }
      return true;
    }

    bool get(AttrIndexEngine &aie)
    {
      engine_set_itr itr = engine_set_.find(aie);
      if (itr == engine_set_.end()) {
        error_log("unexisting index");
        return false;
      }
      aie = *itr;
      return true;
    }

  private:
    engine_set engine_set_;
    Config::Document doc_config_;
  };
}

#endif
