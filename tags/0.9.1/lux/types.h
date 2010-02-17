#ifndef LUX_TYPES_H
#define LUX_TYPES_H

namespace Lux {

  typedef uint32_t doc_id_t;
  typedef uint32_t score_t;

  typedef enum {
    DB_RDONLY = 0x0000,
    DB_RDWR = 0x0002,
    DB_CREAT = 0x0200,
    DB_TRUNC = 0x0400,
  } db_flags_t;

  typedef enum {
    RES_W_POS = 0x0001, // with positions
    RES_WO_POS = 0x0002, // without positions
  } sys_cond_t;

  typedef enum {
    CREATED,
    REGISTERED,
    UNREGISTERED,
    FAILED
  } id_status_t;

}

#endif
