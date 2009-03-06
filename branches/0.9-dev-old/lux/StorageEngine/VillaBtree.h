#ifndef LUX_VILLA_BTREE_H
#define LUX_VILLA_BTREE_H

#include "lux/lux.h"
#include "StorageEngineUtil.h"
#include "DataUnit.h"
#include <villa.h>
#include <cassert>
#include <iostream>
#include <string>

namespace Lux {

    class VillaBtree {
        
    public:
        VillaBtree()
        {}

        VillaBtree(const char *filename, db_flags_t open_flags)
        {
            this->open(filename, open_flags);
        } 

        VillaBtree(std::string filename, db_flags_t open_flags)
        {
            this->open(filename.c_str(), open_flags);
        } 

        bool open(const char *filename, db_flags_t open_flags)
        {
            assert(filename);
            int oflags = 0;
            if (open_flags & DB_RDONLY) {
                oflags = VL_OREADER;
            }
            if (open_flags & DB_RDWR) {
                oflags = VL_OWRITER;
            }
            if (open_flags & DB_CREAT) {
                oflags = VL_OWRITER | VL_OCREAT;
                mkdir(filename);
            }
            if (open_flags & DB_TRUNC) {
                oflags |= VL_OTRUNC;
            }
            if (!(villa_ = vlopen(filename, oflags, VL_CMPLEX))) {
                return false;
            }
            return true;
        }

        bool close()
        {
            if (!vlclose(villa_)) {
                return false;
            }
            villa_ = NULL;
            return true;
        }

        bool put(LuxDataUnit &key, LuxDataUnit &val)
        {
            return __put(key, val, DP_DOVER);
        }

        bool put(LuxDataUnit *key, LuxDataUnit *val)
        {
            return put(*key, *val);
        }

        bool append(LuxDataUnit &key, LuxDataUnit &val)
        {
            return __put(key, val, DP_DCAT);
        }

        bool append(LuxDataUnit *key, LuxDataUnit *val)
        {
            return append(*key, *val);
        }

        bool get(LuxDataUnit &key, LuxDataUnit &val)
        {
            assert(key.get_data());
            int val_size;
            void *val_data;

            val_data = vlget(villa_, static_cast<const char *>(key.get_data()),
                             key.get_size(), &(val_size));

            if (val_data == NULL) {
                return false;
            }

            val.set_cleanup_needed(true);
            val.set_data(val_data);
            val.set_size(static_cast<unsigned int>(val_size));

            return true;
        }

        bool get(LuxDataUnit *key, LuxDataUnit *val)
        {
            return get(*key, *val);
        }

        bool del(LuxDataUnit &key)
        {
            assert(key.get_data());
            if (vlout(villa_, static_cast<const char *>(key.get_data()),
                key.get_size())) {
                return true;
            }
            return false;
        }

        bool del(LuxDataUnit *key)
        {
            return del(*key);
        }

    protected:
        ~VillaBtree()
        {
            if (villa_ != NULL) {
                if (vlclose(villa_)) {
                    villa_ = NULL;
                }
            }
        }

    private:
        VILLA *villa_;

        bool __put(LuxDataUnit &key, LuxDataUnit &val, int mode)
        {
            assert(key.get_data() && val.get_data());
            if (vlput(villa_,
                      static_cast<const char *>(key.get_data()),
                      key.get_size(),
                      static_cast<const char *>(val.get_data()),
                      val.get_size(),
                      mode)) {
                return true;
            }
            return false;
        }
    };
}

#endif
