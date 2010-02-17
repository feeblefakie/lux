#include "index_server.h"
#include "lux/util.h"
#include "update-protocol.pb.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

namespace Lux {
namespace Dist {

  IndexServer::IndexServer(void *p)
  : Handler(p),
    s_(new Lux::Searcher(*(Lux::Engine *) inf_->supp_arg)),
    i_(new Lux::Indexer(*(Lux::Engine *) inf_->supp_arg))
  {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
  }

  IndexServer::~IndexServer(void)
  {}

  bool IndexServer::exec(request_header_t &header,
                          char *body, uint32_t body_len)
  {
    switch (header.command) {
      case COM_QUIT:
      {
        is_persistent_ = false;
        exec_quit();
        is_terminating_ = true;
        break;
      }
      case COM_FIND:
      {
        std::cout << "COM_FIND" << std::endl;
        is_persistent_ = false;
        if (!exec_find(body, body_len)) {
          error_log("exec_find() failed");
          return false;
        }
        break;
      }
      case COM_GET_DOCS:
      {
        std::cout << "COM_GET_DOCS" << std::endl;
        is_persistent_ = false;
        if (!exec_getdoc(body, body_len)) {
          error_log("exec_getdoc() failed.");
          return false;
        }
        break;
      }
      case COM_UPDATE:
      {
        std::cout << "COM_UPDATE" << std::endl;
        // persistent connection with indexer
        is_persistent_ = true;
        if (!exec_update(body, body_len)) {
          error_log("exec_update() failed.");
          return false;
        }
        break;
      }
      default:
        break;
    }
    return true;
  }

  bool IndexServer::exec_find(char *body, uint32_t len)
  {
    search_options_t *opts;
    opts = (search_options_t *) body;
    body += sizeof(search_options_t);

    Lux::SortCondition scond((sort_attr_t) opts->sort_attr,
                             (sort_order_t) opts->sort_order,
                             opts->attr_name);
    Lux::Paging paging(opts->num, opts->page);
    Lux::Condition cond(scond, paging);
    std::string query(body, len - sizeof(search_options_t));
    Lux::IndexResultSet irs = s_->search_by_server(query, cond);

    // check if sorted as requested
    if (((sort_attr_t) opts->sort_attr != cond.sort.attr_type) || 
        ((sort_order_t) opts->sort_order != cond.sort.order_type)) {
      send_error(ATTR_INDEX_NOT_FOUND);
      error_log("requested attribute index is not found.");
      return false;
    }

    char *data = NULL;
    uint32_t size;
    if (!serialize_idx_rs(irs, cond, &data, &size)) {
      return false;
    }

    if (!send_packets(data, size, STATUS_IDX_RS)) {
      return false;
    }

    if (data != NULL) {
      delete [] data;
    }

    // [FIXME] this method is public only for this
    s_->clear_attrs(irs, cond);
    
    return true;
  }

  bool IndexServer::exec_getdoc(char *body, uint32_t len)
  {
    uint32_t num_docs = len / sizeof(doc_id_t);

    DocResultSet drs;
    uint32_t body_len = 0;
    for (uint32_t i = 0; i < num_docs; ++i) {
      doc_id_t id = *(doc_id_t *) body;
      std::string doc = s_->getdoc_by_server(id);
      drs.push_back(DocResult(id, doc));
      body_len += doc.size();
      body += sizeof(doc_id_t);
    }
    body_len += num_docs * (sizeof(doc_id_t) + sizeof(uint32_t));

    char *data = NULL;
    if (!serialize_doc_rs(drs, &data, body_len)) {
      error_log("serialize_doc_rs() failed");
      return false;
    }

    if (!send_packets(data, body_len, STATUS_DOC_RS)) {
      return false;
    }

    if (data != NULL) {
      delete [] data;
    }

    return true;
  }

  bool IndexServer::exec_update(char *body, uint32_t len)
  {
    Lux::Protocol::Updates updates;
    updates.ParseFromArray(body, len);

    /*
    for (int i = 0; i < updates.update_size(); i++) {
      const Lux::Protocol::Update &update = updates.update(i);
      std::cout << "type: " << update.type() << std::endl;

      const Lux::Protocol::Document &document = update.document();
      std::cout << "document: " << std::endl;
      std::cout << "id: " << document.id() << std::endl;

      for (int j = 0; j < document.field_size(); j++) {
        const Lux::Protocol::Field &field = document.field(j);

        std::cout << "field: " << std::endl;
        std::cout << "\tname: " << field.name() << std::endl;
        std::cout << "\tvalue: " << field.value() << std::endl;

      }
      std::cout << "--------------------" << std::endl;
    }
    */

    for (int i = 0; i < updates.update_size(); i++) {
      const Lux::Protocol::Update &update = updates.update(i);
      const Lux::Protocol::Document &document = update.document();
      Lux::Document doc(const_cast<std::string&>(document.id()));

      for (int j = 0; j < document.field_size(); j++) {
        const Lux::Protocol::Field &field = document.field(j);
        doc.add(Lux::Field::create(field.name(), field.value()));
      }

      // [TODO] error handling and exception handling
      switch (update.type()) {
        case Lux::Protocol::Update::ADD:
        {
          i_->add(doc);
          break;
        }
        case Lux::Protocol::Update::DELETE:
        {
          // [TODO]
          break;
        }
        case Lux::Protocol::Update::ALTER:
        {
          // [TODO]
          break;
        }
        default:
        {
          error_log("wrong update type. ignored.");
          break;
        }
      }
    }

    // [TODO] consider the way sending back errors
    // especially in case some of the updates failed.
    if (!send_ok()) {
    }

    return true;
  }

  bool IndexServer::serialize_idx_rs(IndexResultSet &irs, Condition &cond,
                                      char **data, uint32_t *size)
  {
    *size = sizeof(cluster_idx_result_header_t)
            + (sizeof(doc_id_t) + cond.sort.attr_size) * cond.paging.get_num();

    // [TODO] body size limit check
    *data = new char[*size];
    char *p = *data;

    cluster_idx_result_header_t rh;
    rh.total_num = irs.size();
    rh.attr_size = cond.sort.attr_size;
    rh.num_results = cond.paging.get_num();

    memcpy(p, &rh, sizeof(cluster_idx_result_header_t));
    p += sizeof(cluster_idx_result_header_t);

    Paging page = cond.paging;
    for (uint32_t i = page.get_base(); i < page.get_base() + page.get_num(); ++i) {
      memcpy(p, &(irs[i].doc_id), sizeof(doc_id_t)); // docid
      p += sizeof(doc_id_t);
      if (cond.sort.attr_type == SORT_SCORE) {
        memcpy(p, &(irs[i].score), sizeof(score_t)); // score
      } else {
        memcpy(p, irs[i].attr, cond.sort.attr_size); // attribute
      }
      p += cond.sort.attr_size;
    }

    return true;
  }

  bool IndexServer::serialize_doc_rs(DocResultSet &drs,
                                      char **data, uint32_t size)
  {
    // [TODO] body size limit check
    *data = new char[size];
    char *p = *data;

    DocResultSet::iterator itr_end = drs.end();
    for (DocResultSet::iterator itr = drs.begin(); itr != itr_end; ++itr) {
      memcpy(p, &itr->id, sizeof(doc_id_t));
      p += sizeof(doc_id_t);
      uint32_t len = itr->data.size();
      memcpy(p, &len, sizeof(uint32_t));
      p += sizeof(uint32_t);
      memcpy(p, itr->data.c_str(), len);
      p += len;
    }

    return true;
  }

}
}
