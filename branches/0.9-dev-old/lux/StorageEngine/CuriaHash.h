#ifndef LUX_CURIA_HASH_H
#define LUX_CURIA_HASH_H

#include "lux/lux.h"
#include "StorageEngine.h"
#include "DataUnit.h"
#include <curia.h>
#include <cassert>
#include <iostream>

namespace Lux {

    class CuriaHash {
        
    public:
        CuriaHash()
        {}

        CuriaHash(const char *filename, db_flags_t open_flags)
        {
            this->open(filename, open_flags);
        }

        CuriaHash(std::string filename, db_flags_t open_flags)
        {
            this->open(filename.c_str(), open_flags);
        } 

        bool open(const char *filename, db_flags_t open_flags)
        {
            assert(filename);
            int oflags = 0;
            if (open_flags & DB_RDONLY) {
                oflags = CR_OREADER;
            }
            if (open_flags & DB_RDWR) {
                oflags = CR_OWRITER;
            }
            if (open_flags & DB_CREAT) {
                oflags = CR_OWRITER | CR_OCREAT;
                mkdir(filename);
            }
            if (open_flags & DB_TRUNC) {
                oflags |= CR_OTRUNC;
            }
            if (!(curia_ = cropen(filename, oflags, -1, -1))) {
                return false;
            }
            return true;
        }

        bool close()
        { 
            if (!crclose(curia_)) {
                return false;
            }
            curia_ = NULL;
            return true;
        }

        bool put(LuxDataUnit &key, LuxDataUnit &val)
        {
            return __put(key, val, CR_DOVER);
        }

        bool put(LuxDataUnit *key, LuxDataUnit *val)
        {
            return put(*key, *val);
        }

        bool append(LuxDataUnit &key, LuxDataUnit &val)
        {
            return __put(key, val, CR_DCAT);
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

            val_data = crget(curia_, static_cast<const char *>(key.get_data()),
                             key.get_size(), 0, -1, &(val_size));

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
            if (crout(curia_, static_cast<const char *>(key.get_data()),
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
        ~CuriaHash()
        {
            if (curia_ != NULL) {
                if (crclose(curia_)) {
                    curia_ = NULL;
                }
            }
        }

    private:
        CURIA *curia_;

        bool __put(LuxDataUnit &key, LuxDataUnit &val, int mode)
        {
            assert(key.get_data() && val.get_data());
            if (crput(curia_,
                      static_cast<const char *>(key.get_data()),
                      key.get_size(),
                      static_cast<const char *>(val.get_data()),
                      val.get_size(),
                      mode)
                ) {
                return true;
            }
            return false;
        }
    };
}

#endif
