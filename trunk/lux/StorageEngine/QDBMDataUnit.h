#ifndef LUX_QDBM_DATAUNIT_H
#define LUX_QDBM_DATAUNIT_H

#include <cstdlib>
#include <cassert>
#include <iostream>

namespace Lux {

    class QDBMDataUnit {

    public:
        QDBMDataUnit(bool is_cleanup_needed = false)
        : is_cleanup_needed_(is_cleanup_needed)
        {}

        QDBMDataUnit(void *data, unsigned int size, bool is_cleanup_needed = false)
        : data_(data), size_(size), is_cleanup_needed_(is_cleanup_needed)
        {
            assert(data_ && size_ > 0);         
        } 

        void set_data(void *data)
        {
            assert(data);         
            data_ = data; 
        }

        void set_size(unsigned int size)
        {
            assert(size > 0);         
            size_ = size;
        }

        void set(void *data, unsigned int size)
        {
            assert(data && size > 0);         
            set_data(data);
            set_size(size);
        }

        void *get_data(void)
        {
            return data_;
        }

        unsigned int get_size(void)
        {
            return size_;
        }

        void set_cleanup_needed(bool is_cleanup_needed)
        {
            is_cleanup_needed_ = is_cleanup_needed;
        }

    protected:
        ~QDBMDataUnit() {
            if (is_cleanup_needed_) {     
                cleanup();
            }
        }

    private:
        void *data_;
        unsigned int size_;
        bool is_cleanup_needed_;

        void cleanup(void)
        {
            if (data_ != NULL) {
                // data from QDBM is allocated by malloc, so do free
                free(data_);
                data_ = NULL;
            }
        }
    };
}

#endif
