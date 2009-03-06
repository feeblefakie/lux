#include "indexer.h"
#include "protocol.h"
#include "message_queue.h"
#include "update-protocol.pb.h"
#include "lux/util.h"

namespace Lux {
namespace Dist {

  Indexer::Indexer(void *p)
  : client_((client_thread_inf_t *) p),
    server_((server_inf_t *) client_->server)
  {}

  Indexer::~Indexer(void)
  {}

  void Indexer::process(void)
  {
    MessageQueue<Lux::Protocol::Updates> *mq = 
        (MessageQueue<Lux::Protocol::Updates> *) client_->queue;
    uint32_t max_body_size = 1 << PACKET_BODY_MAX_BITS;
    char *body = new char[max_body_size];

    sock_t s;
    if (!sock_connect(s, server_->ip, server_->port)) {
      error_log("sock_connect() failed.");
    }

    while (1) {
      if (mq->is_queue_empty()) {
        if (client_->is_terminating) {
          return;
        }
        sleep(1);
        continue;
      }

      std::cout << server_->id << " - Message: " << std::endl;
      Lux::Protocol::Updates updates = mq->dequeue_message();
      int body_size = updates.ByteSize();
      memset(body, 0, max_body_size);
      updates.SerializeToArray(body, body_size);

      request_header_t req_h = {body_size, 0, COM_UPDATE};
      struct iovec v[2];
      v[0].iov_base = &req_h;
      v[0].iov_len = sizeof(req_h);
      v[1].iov_base = body;
      v[1].iov_len = body_size;

      if (lut_writev(s, v, 2) < 0) {
        perror("writev failed.");
      }

      response_header_t resp_h;
      if (lut_recv(s, (char *) &resp_h, sizeof(resp_h), 0) <= 0) {
        error_log("lut_recv() failed");
      }   
      if (resp_h.status != STATUS_OK) {
        error_log("indexing failed.");
      }
    }
    close(s);
  }

}
}
