#include "luxio_array.h"

namespace Lux {

  LuxIOArray::LuxIOArray(void)
  : ary_(new IO::Array(IO::CLUSTER))
  {}

  bool LuxIOArray::open(const char *filename, db_flags_t open_flags)
  {
    assert(filename);
    if (open_flags & DB_CREAT) {
      _mkdir(filename);
    }
    if (!ary_->open(filename, (IO::db_flags_t) open_flags)) {
      return false;
    }
    return true;
  }

  bool LuxIOArray::close(void)
  {
    if (!ary_->close()) {
      return false;
    }
    return true;
  }

  void LuxIOArray::set_index_type(IO::db_index_t index_type,
                                  uint8_t data_size)
  {
    ary_->set_index_type(index_type, data_size);
  }

  bool LuxIOArray::put(uint32_t index, LuxDataUnit &val)
  {
    if (!ary_->put(index, val.get_data(), val.get_size())) {
      return false;
    }
    return true;
  }

  bool LuxIOArray::put(uint32_t index, LuxDataUnit *val)
  {
    return put(index, *val);
  }

  bool LuxIOArray::append(uint32_t index, LuxDataUnit &val)
  {
    if (!ary_->put(index, val.get_data(), val.get_size(), IO::APPEND)) {
      return false;
    }
    return true;
  }

  bool LuxIOArray::append(uint32_t index, LuxDataUnit *val)
  {
    return append(index, *val);
  }

  bool LuxIOArray::get(uint32_t index, LuxDataUnit &val)
  {
    IO::data_t *val_data = ary_->get(index);
    if (val_data == NULL) {
      return false;
    }

    val.set_cleanup_needed(true);
    val.set_data((void *) val_data->data);
    val.set_size(val_data->size);
    delete val_data;

    return true;
  }

  bool LuxIOArray::get(uint32_t index, LuxDataUnit *val)
  {
    return get(index, *val);
  }

  bool LuxIOArray::del(uint32_t index)
  {
    if (!ary_->del(index)) {
      return true;
    }
    return false;
  }

  LuxIOArray::~LuxIOArray(void)
  {
    if (ary_ != NULL) {
      ary_->close();
      delete ary_;
      ary_ = NULL;
    }
  }

}
