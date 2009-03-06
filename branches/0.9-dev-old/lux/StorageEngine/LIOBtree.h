#ifndef LUX_LIOBTREE_H
#define LUX_LIOBTREE_H

#include <luxio/btree.h>
#include "lux/lux.h"
#include "DataUnit.h"
#include <cassert>
#include <iostream>
#include <string>

namespace Lux {

  class LIOBtree {
      
  public:
    LIOBtree()
    : bt_(new IO::Btree(IO::NONCLUSTER))
    {}

    bool open(const char *filename, db_flags_t open_flags)
    {
      assert(filename);
      if (open_flags & DB_CREAT) {
        _mkdir(filename);
      }
      if (!bt_->open(filename, open_flags)) {
        return false;
      }
      return true;
    }

    bool close()
    {
      if (!bt_->close()) {
        return false;
      }
      return true;
    }

    void set_index_type(IO::db_index_t index_type)
    {
      bt_->set_index_type(index_type);
    }

    void set_cmp_func(IO::CMP cmp)
    {
      bt_->set_cmp_func(cmp);
    }

    bool put(LuxDataUnit &key, LuxDataUnit &val)
    {
      if (!bt_->put(key.get_data(), key.get_size(),
               val.get_data(), val.get_size())) {
        return false;
      }
      return true;
    }

    bool put(LuxDataUnit *key, LuxDataUnit *val)
    {
      return put(*key, *val);
    }

    bool append(LuxDataUnit &key, LuxDataUnit &val)
    {
      if (!bt_->put(key.get_data(), key.get_size(),
               val.get_data(), val.get_size(), IO::APPEND)) {
        return false;
      }
      return true;
    }

    bool append(LuxDataUnit *key, LuxDataUnit *val)
    {
      return append(*key, *val);
    }

    bool get(LuxDataUnit &key, LuxDataUnit &val)
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

    bool get(LuxDataUnit *key, LuxDataUnit *val)
    {
      return get(*key, *val);
    }

    bool del(LuxDataUnit &key)
    {
      assert(key.get_data());
      if (!bt_->del(key.get_data(), key.get_size())) {
        return true;
      }
      return false;
    }

    bool del(LuxDataUnit *key)
    {
      return del(*key);
    }

  protected:
    ~LIOBtree()
    {
      if (bt_ != NULL) {
        bt_->close();
        delete bt_;
        bt_ = NULL;
      }
    }

  private:
    IO::Btree *bt_;
  };

}

#endif
