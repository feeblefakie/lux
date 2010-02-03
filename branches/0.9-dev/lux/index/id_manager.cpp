#include "id_manager.h"
#include "lux/document/document.h"

namespace Lux {

  IDManager::IDManager(void)
  : manager_(new LuxBtreeStorage)
  {}

  IDManager::~IDManager() 
  {
    close();
  }

  bool IDManager::open(std::string dir, db_flags_t oflags)
  {
    oflags_ = oflags;
    manager_->set_index_type(IO::CLUSTER);
    std::string db_name = dir + "/" + LUX_ID_DB;
    if (!manager_->open(db_name.c_str(), oflags)) {
      error_log("IDManager::open failed");
      return false;
    }
    LuxDataUnit id_val;
    id_key_.set(const_cast<char *>(IID), strlen(IID));

    if (manager_->get(id_key_, id_val)) {
      curr_id_ = *(doc_id_t *) id_val.get_data();
    } else {
      curr_id_ = 0;
    }
    std::cout << "curr_id_: " << curr_id_ << std::endl;
    return true;
  }

  bool IDManager::close(void)
  {
    if (oflags_ ^ DB_RDONLY) {
      LuxDataUnit id_val(&curr_id_, sizeof(doc_id_t));
      if (!manager_->put(id_key_, id_val)) {
        error_log("update id failed.");
        return false;
      }
    }
    return true;
  }

  id_status_t IDManager::set_id(Document *doc, bool create_if_new)
  {
    return set_id(*doc, create_if_new);
  }

  id_status_t IDManager::set_id(Document &doc, bool create_if_new)
  {
    id_status_t id_status;
    LuxDataUnit iid;
    std::string e = doc.get_eid();  
    LuxDataUnit eid(const_cast<char *>(e.c_str()), e.size());

    if (manager_->get(eid, iid)) {
      doc.set_id(*(doc_id_t *) iid.get_data());
      return REGISTERED;
    }
    if (!create_if_new) {
      return UNREGISTERED;
    }

    ++curr_id_;
    iid.set((void *) &curr_id_, sizeof(doc_id_t));
    if (!manager_->put(eid, iid)) {
      return FAILED;
    }
    doc.set_id(curr_id_);

    LuxDataUnit id_val(&curr_id_, sizeof(doc_id_t));
    if (!manager_->put(id_key_, id_val)) {
      error_log("update id failed.");
      manager_->del(eid);
      return FAILED;
    }
    return CREATED;
  }

}
