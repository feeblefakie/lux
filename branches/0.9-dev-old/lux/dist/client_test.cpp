#include "common.h"
#include "protocol.h"
#include "lux/search.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace Lux;

int main(int argc, char *argv[])
{
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " host_or_ip" << std::endl;
    exit(1);
  }

  uint32_t dst_ip;
  if ((dst_ip = inet_addr(argv[1])) == INADDR_NONE) {
    struct hostent *he;
    if ((he = gethostbyname(argv[1])) == NULL) {
      error_log("gethostbyname failed");
      exit(1);
    }
    memcpy((char *) &dst_ip, (char *) he->h_addr, he->h_length);
  }

  while (1) {
    int s;
    struct sockaddr_in server;
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("socket");
      exit(1);
    }

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = dst_ip;
    server.sin_port = htons(sport);

    if (lut_connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
      perror("connect");
      exit(1);
    }
    std::cout << "connected" << std::endl;

    char body[256];
    memset(body, 0, 256);
    strcpy(body, "vaio AND sony");

    uint32_t body_len = sizeof(search_options_t) + strlen(body) + 1; // null terminated

    // request is sent to all the servers
    request_header_t header = {body_len, 0, COM_FIND};
    if (lut_send(s, &header, sizeof(header), 0) < 0) {
      std::cerr << "send failed" << std::endl;
      break;
    }
    search_options_t options = {SORT_SCORE, DESC, 3, 0};
    if (lut_send(s, (void *) &options, sizeof(options), 0) < 0) {
      std::cerr << "send failed" << std::endl;
      break;
    }
    if (lut_send(s, (void *) body, strlen(body) + 1, 0) < 0) {
      std::cerr << "send failed" << std::endl;
      break;
    }

    // must merge all the response
    response_header_t res;
    if (lut_recv(s, (char *) &res, sizeof(res), 0) <= 0) {
      return false;
    }

    if (res.status != STATUS_IDX_RS) {
      error_log("error happened. closeing the active socket");
      close(s);
      continue;
    }

    std::cout << "got body - len: " << res.length << std::endl;
    char *buf = new char[res.length];
    if (lut_recv(s, buf, res.length, 0) <= 0) {
      return false;
    }

    uint32_t *total_num_p = (uint32_t *) buf;
    buf += sizeof(uint32_t);
    std::cout << "total: " << *total_num_p << std::endl;

    uint32_t *attr_size = (uint32_t *) buf;
    buf += sizeof(uint32_t);

    IndexResultSet irs;
    irs.reserve(options->num);
    uint32_t len = res.length - sizeof(uint32_t) * 2;
    if (options.sort_attr == SORT_SCORE) {
      while (len > 0) {
        IndexResult ir;
        ir.doc_id = *(doc_id_t *) buf;
        buf += sizeof(doc_id_t);
        ir.score = (score_t *) buf;
        buf += sizeof(score_t);
        len -= sizeof(doc_id_t) + sizeof(score_t);
        irs.push_back(ir);
      }
    } else {
      while (len > 0) {
        IndexResult ir;
        ir.doc_id = *(doc_id_t *) buf;
        buf += sizeof(doc_id_t);
        memcpy(&(ir.attr), buf, *attr_size);
        buf += *attr_size;
        len -= sizeof(doc_id_t) + *attr_size;
        irs.push_back(ir);
      }
    }

    close(s);
    usleep(500000);
  }

}
