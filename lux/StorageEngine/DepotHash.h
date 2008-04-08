#ifndef LUX_DEPOTHASH_H
#define LUX_DEPOTHASH_H

#include "lux/lux.h"
#include "StorageEngineUtil.h"
#include "DataUnit.h"
#include <depot.h>
#include <cassert>
#include <iostream>

namespace Lux {

    class DepotHash {
        
    public:
        DepotHash()
        {}

        DepotHash(const char *filename, db_flags_t open_flags)
        {
            this->open(filename, open_flags);
        }

        DepotHash(std::string filename, db_flags_t open_flags)
        {
            this->open(filename.c_str(), open_flags);
        } 

        bool open(const char *filename, db_flags_t open_flags)
        {
            assert(filename);
            unsigned int oflags = 0;
            if (open_flags & DB_RDONLY) {
                oflags = DP_OREADER;
            }
            if (open_flags & DB_RDWR) {
                oflags = DP_OWRITER;
            }
            if (open_flags & DB_CREAT) {
                oflags = DP_OWRITER | DP_OCREAT;
                mkdir(filename);
            }
            if (open_flags & DB_TRUNC) {
                oflags |= DP_OTRUNC;
            }
            if (!(depot_ = dpopen(filename, oflags, -1))) {
                return false;
            }
            return true;
        }

        bool close()
        {
            if (!dpclose(depot_)) {
                return false;
            }
            depot_ = NULL;
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

            val_data = dpget(depot_, static_cast<const char *>(key.get_data()),
                             key.get_size(), 0, -1, &(val_size));

            if (val_data == NULL) {
                return false;
            }

            /*
             * LuxDataUnit val must be cleaned up because qdbm returns 
             * dynamically allocated date in the library.
             */
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
            if (dpout(depot_, static_cast<const char *>(key.get_data()),
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
        ~DepotHash()
        {
            if (depot_ != NULL) {
                if (dpclose(depot_)) {
                    depot_ = NULL;
                }
            }
        }

    private:
        DEPOT *depot_;

        bool __put(LuxDataUnit &key, LuxDataUnit &val, int mode)
        {
            assert(key.get_data() && val.get_data());
            if (dpput(depot_,
                      static_cast<const char *>(key.get_data()), key.get_size(),
                      static_cast<const char *>(val.get_data()), val.get_size(),
                      mode)) {
                return true;
            }
            return false;
        }

    };
}

#endif
