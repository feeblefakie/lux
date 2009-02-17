#ifndef LUX_IDMANAGER_H
#define LUX_IDMANAGER_H

#include "lux/lux.h"
#include "lux/StorageEngine/StorageEngine.h"
#include <iostream>
#include <string>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

#define IID "__IID__"

namespace Lux {

  class IDManager {
      
  public:
    IDManager(void)
    : manager_(new LuxBtreeStorage)
    {}

    ~IDManager() 
    {
      close();
    }

    bool open(std::string dir, db_flags_t oflags = DB_CREAT)
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

    bool close(void)
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

    id_status_t set_id(Document *doc)
    {
      return set_id(*doc);
    }

    id_status_t set_id(Document &doc)
    {
      id_status_t id_status;
      LuxDataUnit iid;
      std::string e = doc.get_eid();  
      LuxDataUnit eid(const_cast<char *>(e.c_str()), e.size());

      if (manager_->get(eid, iid)) {
        id_status = REGISTERED;
        doc.set_id(*(doc_id_t *) iid.get_data());
      } else {
        ++curr_id_;
        iid.set((void *) &curr_id_, sizeof(doc_id_t));
        if (!manager_->put(eid, iid)) {
          id_status = FAILED;
          return id_status;
        }
        id_status = NEW;
        doc.set_id(curr_id_);

        LuxDataUnit id_val(&curr_id_, sizeof(doc_id_t));
        if (!manager_->put(id_key_, id_val)) {
          error_log("update id failed.");
        }
      }
      return id_status;
    }

  private:
    doc_id_t curr_id_;
    LuxDataUnit id_key_;
    db_flags_t oflags_;
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<LuxBtreeStorage> manager_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<LuxBtreeStorage> manager_;
#endif
  };
}

#endif
