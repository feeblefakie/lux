#include "Indexer.h"
#include "lux/Document/Document.h"
#include "lux/Storage/InvertedIndex.h"

namespace Lux {

    Indexer::Indexer(std::string storage_dir, DocumentDefinition &doc_def)
    : doc_def_(doc_def),
      lhs_(new LuxHashStorage(storage_dir + "/" + LUX_META_DB, DB_CREAT)),
      ds_(new LuxDocStorage(storage_dir, DB_CREAT, doc_def)),
      si_(new InvertedIndex(storage_dir, DB_CREAT, doc_def))
    {
      LuxDataUnit id_val;
      id_key_.set(const_cast<char *>(INTERNALID), strlen(INTERNALID));

      if (lhs_->get(id_key_, id_val)) {
        memcpy(&curr_int_doc_id_, id_val.get_data(), id_val.get_size());
      } else {
        curr_int_doc_id_ = 0;
      }
    }

    Indexer::~Indexer()
    {
      // everything is automatically deleted.

      // commented out for now
      si_->flush();

      LuxDataUnit id_val(&curr_int_doc_id_, sizeof(doc_id_t));
      if (lhs_->put(id_key_, id_val)) {
        //std::cout << "id_key is updated." << std::endl;
      } else {
        //std::cout << "updating id_key failed." << std::endl;
      }
    }

    void Indexer::add(Document &doc)
    {
      // create internal id  external_id_ -> internal_id_
      doc_id_t internal_id;
      id_status_t status = ext_to_int(doc.get_external_id(), &internal_id);
      if (status == ID_REGISTERED) {
        std::cerr << "updating existing document is not supported for now." 
                  << std::endl;
        return;
      }
      doc.set_internal_id(internal_id);
      //std::cout << doc.get_internal_id() << " is added." << std::endl;

      // add to DocumentStorage, SearchIndex, AttrIndex
      ds_->add(doc);
      si_->add(doc);
      //ai_->add(doc, int_doc_id);

      //si_->print_buffer();
      //std::cout << "buffer_size: " << si_->get_buffer_size() << std::endl;

      if (si_->get_buffer_size() > INDEX_BUFFER_THRESHOLD) {
        si_->flush();
      }
    }

    id_status_t Indexer::ext_to_int(std::string e, doc_id_t *i)
    {
      LuxDataUnit int_id;
      LuxDataUnit ext_id(const_cast<char *>(e.c_str()), e.size());

      // external id -> internal id
      if (lhs_->get(ext_id, int_id)) {
        // already registered id
        //memcpy(&int_doc_id, int_id.get_data(), int_id.get_size());
        return ID_REGISTERED;
      }

      // new id
      *i = ++curr_int_doc_id_; 
      int_id.set(i, sizeof(doc_id_t));
      lhs_->put(ext_id, int_id);
      return ID_NEW;
    }
}
