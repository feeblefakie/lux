#include "attr_index_array.h"
#include "lux/document/document.h"

namespace Lux {

  /**
   * class AttrIndexEngine
   */
  AttrIndexEngine::AttrIndexEngine(const std::string &attr_name_,
        Config::Field::AttrIndexType attr_type_, uint32_t attr_size_)
  : attr_name(attr_name_),
    attr_type(attr_type_),
    attr_size(attr_size_),
    engine(new LuxArrayStorage)
  {}

  // for comparison
  AttrIndexEngine::AttrIndexEngine(const std::string &attr_name_)
  : attr_name(attr_name_)
  {}

  bool AttrIndexEngine::open(const std::string file, db_flags_t open_params_)
  {
    engine->set_index_type(IO::CLUSTER, attr_size);
    return engine->open(file.c_str(), open_params_);
  }

  bool AttrIndexEngine::close(void)
  {
    return engine->close();
  }

  /**
   * class AttrIndexArray
   */
  AttrIndexArray::AttrIndexArray(Config::Document &doc_config)
  : doc_config_(doc_config)
  {}

  bool AttrIndexArray::open(std::string storage_dir, db_flags_t open_params)
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

  bool AttrIndexArray::close(void)
  {
    return true;
  }

  bool AttrIndexArray::add(const Document *doc)
  {
    return add(*doc);
  }

  bool AttrIndexArray::add(const Document &doc)
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
        return false;
      }
    }
    return true;
  }

  bool AttrIndexArray::get(AttrIndexEngine &aie)
  {
    engine_set_itr itr = engine_set_.find(aie);
    if (itr == engine_set_.end()) {
      error_log("unexisting index");
      return false;
    }
    aie = *itr;
    return true;
  }

}
