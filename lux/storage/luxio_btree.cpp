#include "luxio_btree.h"

namespace Lux {

  LuxIOBtree::LuxIOBtree(void)
  : bt_(new IO::Btree(IO::NONCLUSTER))
  {}

  bool LuxIOBtree::open(const char *filename, db_flags_t open_flags)
  {
    assert(filename);
    if (open_flags & DB_CREAT) {
      _mkdir(filename);
    }
    if (!bt_->open(filename, (IO::db_flags_t) open_flags)) {
      return false;
    }
    return true;
  }

  bool LuxIOBtree::close(void)
  {
    if (!bt_->close()) {
      return false;
    }
    return true;
  }

  void LuxIOBtree::set_index_type(IO::db_index_t index_type)
  {
    bt_->set_index_type(index_type);
  }

  void LuxIOBtree::set_cmp_func(IO::CMP cmp)
  {
    bt_->set_cmp_func(cmp);
  }

  bool LuxIOBtree::put(LuxDataUnit &key, LuxDataUnit &val)
  {
    if (!bt_->put(key.get_data(), key.get_size(),
             val.get_data(), val.get_size())) {
      return false;
    }
    return true;
  }

  bool LuxIOBtree::put(LuxDataUnit *key, LuxDataUnit *val)
  {
    return put(*key, *val);
  }

  bool LuxIOBtree::append(LuxDataUnit &key, LuxDataUnit &val)
  {
    if (!bt_->put(key.get_data(), key.get_size(),
             val.get_data(), val.get_size(), IO::APPEND)) {
      return false;
    }
    return true;
  }

  bool LuxIOBtree::append(LuxDataUnit *key, LuxDataUnit *val)
  {
    return append(*key, *val);
  }

  bool LuxIOBtree::get(LuxDataUnit &key, LuxDataUnit &val)
  {
    assert(key.get_data());
    IO::data_t *val_data = bt_->get(key.get_data(), key.get_size());
    if (val_data == NULL) {
      return false;
    }

    val.set_cleanup_needed(true);
    val.set_data((void *) val_data->data);
    val.set_size(val_data->size);
    delete val_data;

    return true;
  }

  bool LuxIOBtree::get(LuxDataUnit *key, LuxDataUnit *val)
  {
    return get(*key, *val);
  }

  bool LuxIOBtree::del(LuxDataUnit &key)
  {
    assert(key.get_data());
    if (!bt_->del(key.get_data(), key.get_size())) {
      return true;
    }
    return false;
  }

  bool LuxIOBtree::del(LuxDataUnit *key)
  {
    return del(*key);
  }

  LuxIOBtree::~LuxIOBtree(void)
  {
    if (bt_ != NULL) {
      bt_->close();
      delete bt_;
      bt_ = NULL;
    }
  }

}
