#ifndef LUX_DIST_HANDLER_H
#define LUX_DIST_HANDLER_H

#include "common.h"
#include "protocol.h"

namespace Lux {
namespace Dist {

  class Handler {
  public:
    Handler(void *p);
    virtual ~Handler(void) = 0;
    virtual void process(void);

  protected:
    sock_t con_;
    thread_inf_t *inf_;
    global_thread_stats_t *stats_;
    bool is_terminating_;
    bool is_persistent_;

    int recv_packets(request_header_t &header,
                      char **body, uint32_t *len);
    bool send_packets(char *data, uint32_t size, resp_status_t status);
    bool send_ok(void);
    bool send_error(err_code_t err_code);
    virtual bool exec(request_header_t &header,
                      char *body, uint32_t body_len) = 0;
    virtual bool exec_quit(void);
    void inc_thread_(void);
    void dec_thread_(void);
  };

}
}

#endif
