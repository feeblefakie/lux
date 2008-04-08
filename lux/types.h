#ifndef LUX_TYPES_H
#define LUX_TYPES_H

namespace Lux {

  typedef unsigned int uint_t;
  typedef uint_t doc_id_t;

  typedef enum {
    DB_RDONLY = 0x0000,
    DB_RDWR = 0x0002,
    DB_CREAT = 0x0200,
    DB_TRUNC = 0x0400,
  } db_flags_t;

}

#endif
