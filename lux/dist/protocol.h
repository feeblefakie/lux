#ifndef LUX_DIST_PROTOCOL_H
#define LUX_DIST_PROTOCOL_H

#include "lux/lux.h"

#define PACKET_BODY_MAX_BITS 28

namespace Lux {
namespace Dist {


  /* common headers in all request packet */
  typedef struct {
    uint32_t length: PACKET_BODY_MAX_BITS;
    uint32_t comp_length: PACKET_BODY_MAX_BITS;
    uint32_t command: 8;
  } request_header_t;

  /* common headers in all response packet */
  typedef struct {
    uint32_t length: PACKET_BODY_MAX_BITS;
    uint32_t comp_length: PACKET_BODY_MAX_BITS;
    uint32_t status: 8;
  } response_header_t;

#pragma pack(2)
  typedef struct {
    uint8_t sort_attr;
    uint8_t sort_order;
    char attr_name[16];
    uint16_t num;
    uint16_t page;
  } search_options_t;

  typedef struct {
    uint32_t total_num;
    uint16_t attr_size;
    uint16_t num_results;
  } cluster_idx_result_header_t;
#pragma pack()

  typedef struct {
    uint32_t total_num;
    uint16_t base;
    uint16_t num;
  } proxy_doc_result_header_t;

  typedef enum {
    COM_QUIT,
    COM_FIND,
    COM_GET_DOCS,
    COM_DISPATCH,
    COM_UPDATE
  } command_t;

  typedef enum {
    STATUS_OK,
    STATUS_ERR,
    STATUS_IDX_RS,
    STATUS_DOC_RS,
    STATUS_RS
  } resp_status_t;

  typedef enum {
    CLUSTER_ERROR,
    RUNTIME_ERROR,
    ATTR_INDEX_NOT_FOUND
  } err_code_t;

}
}

#endif
