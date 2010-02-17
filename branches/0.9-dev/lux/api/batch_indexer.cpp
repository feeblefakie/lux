#include "batch_indexer.h"
#include "batch_indexer-impl.h"
#include "lux/document/document.h"
#include "lux/index/in_memory_index.h"
#include <algorithm>

namespace Lux {

  BatchIndexer::BatchIndexer(Config::Document &doc_config)
  : index_buffer_threshold_(1024 * 30),
    flush_counter_(0),
    pimpl_(new BatchIndexerImpl(doc_config))
  {}

  BatchIndexer::~BatchIndexer()
  {}

  bool BatchIndexer::open(std::string dir)
  {
    dir_ = dir;
    db_flags_t oflags = Lux::DB_CREAT;
    if (!pimpl_->ai->open(dir, oflags)) {
      error_log("opening attribute index failed.");
      return false;
    }
    if (!pimpl_->ds->open(dir, oflags)) {
      error_log("opening document storage failed.");
      return false;
    }
    if (!pimpl_->im->open(dir, oflags)) {
      error_log("opening id manager failed.");
      return false;
    }
    return true;
  }

  bool BatchIndexer::close(void)
  {}

  void BatchIndexer::set_index_buffer_threshold(uint32_t mega_bytes)
  {
    // [TODO] should be multiplied by 2 for estimate
    index_buffer_threshold_ = mega_bytes * 1024 * 1024;
  }

  bool BatchIndexer::add(Document &doc)
  {
    id_status_t status = pimpl_->im->set_id(doc);
    if (status == FAILED) {
      return false;
    } else if (status == REGISTERED) {
      // [TODO] update or do something
      return true;
    } else {
      if (!pimpl_->mi->add(doc)) {
        error_log("adding document into in-memory index failed.");
        return false;
      }
      if (!pimpl_->ds->add(doc)) {
        error_log("adding document into document storage failed.");
        return false;
      }
      if (!pimpl_->ai->add(doc)) {
        error_log("adding document into attribute index failed.");
        return false;
      }
    }

    // check an estimate size of actually allocated by the tree structure
    if (pimpl_->mi->get_buffer_size() * 2 > index_buffer_threshold_) {
      std::cout << "flushing temporary index - " << flush_counter_  << std::endl;
      if (!flush()) {
        error_log("flushing temporary indexes failed.");
        return false;
      }
    }
    return true;
  }

  bool BatchIndexer::add(Document *doc)
  {
    return add(*doc);
  }

  bool BatchIndexer::merge(void)
  {
    if (pimpl_->mi->get_buffer_size() > 0) { 
      std::cout << "flushing temporary index - " << flush_counter_  << std::endl;
      if (!flush()) {
        error_log("flushing temporary indexes failed.");
        return false;
      }
    }

    std::cout << "multi-way merging the indexes." << std::endl;
    // multi-way merging
    pimpl_->mi->init_iter();
    while (pimpl_->mi->has_next()) {

      int batch_num = 20;
      off_t offs[batch_num]; // for offsets
      int fds[batch_num];
      uint32_t nums[batch_num];
      for (uint32_t i = 0; i < flush_counter_; ++i) {
        char sufbuf[32];
        sprintf(sufbuf, "%d", i);
        std::string index_to = pimpl_->mi->get_next_index_to();
        std::string tmp_file = dir_ + "/" + index_to +
                               ".index.tmp." + std::string(sufbuf);
        int fd = Lux::_open(tmp_file.c_str(), DB_RDONLY);
        if (fd < 0) { return false; }
        fds[i] = fd;
        offs[i] = 0;
      }

      TLA heap;
      for (uint32_t i = 0; i < flush_counter_; ++i) {
        TLA tla;
        tla.reserve(batch_num);
        if (!get_records(tla, fds, offs, i, batch_num)) {
          error_log("get_records failed.");
        }
        nums[i] = batch_num;
        TLAITR itr_end = tla.end();
        for (TLAITR itr = tla.begin(); itr != itr_end; ++itr) {
          heap.push_back(*itr);
        }
      }
      make_heap(heap.begin(), heap.end(), greater_term());
      do {
        term_list tl = heap.at(0);
        uint32_t idx = tl.idx;

        // [TODO] write and free
        // check if it is the same term as the previous one to combine those lists
        // sort by idx if needed
        std::cout << tl.term << std::endl;

        pop_heap(heap.begin(), heap.end(), greater_term());
        heap.pop_back();
        if (--nums[idx] == 0) {
          TLA tla;
          if (!get_records(tla, fds, offs, idx, batch_num)) {
            error_log("get_records failed.");
          }
          nums[idx] = batch_num;
          TLAITR itr_end = tla.end();
          for (TLAITR itr = tla.begin(); itr != itr_end; ++itr) {
            heap.push_back(*itr);
            push_heap(heap.begin(), heap.end(), greater_term());
          }
        }
      } while (!heap.empty());
    }

    return true;
  }

  bool BatchIndexer::flush(void)
  {
    pimpl_->mi->init_iter();
    while (pimpl_->mi->has_next()) {

      char sufbuf[32];
      sprintf(sufbuf, "%d", flush_counter_);
      std::string index_to = pimpl_->mi->get_next_index_to();
      std::cout << "flushing " << index_to << "-" << flush_counter_ << std::endl;
      std::string tmp_file = dir_ + "/" + index_to +
                             ".index.tmp." + std::string(sufbuf);
      int fd = Lux::_open(tmp_file.c_str(), DB_CREAT);
      if (fd < 0) { return false; }

      bool is_failed = false;
      off_t offset = 0;
      TMP::TermBufferSet tb = pimpl_->mi->get_term_buffer_set(index_to);
      TMP::TBIterator tb_itr = tb.begin(); 
      while (tb_itr != tb.end()) {
        // write a term
        const char *termp = tb_itr->term.text_.c_str();
        size_t term_size = tb_itr->term.text_.size(); 
        if (!Lux::_pwrite(fd, &term_size, sizeof(size_t), offset) ||
            !Lux::_pwrite(fd, termp, term_size, offset + sizeof(size_t))) {
          is_failed = true; 
          break; 
        }
        offset += sizeof(size_t) + term_size;
        // write posting list for the term
        std::vector<uint8_t> bs = vb_encode(const_cast<pos_list_t&>(tb_itr->buffer));
        char *listp = (char *) &(bs[0]);
        size_t list_size = bs.size();
        if (!Lux::_pwrite(fd, &list_size, sizeof(size_t), offset) ||
            !Lux::_pwrite(fd, listp, list_size, offset + sizeof(size_t))) {
          is_failed = true; 
          break; 
        }
        offset += bs.size();
        ++tb_itr;
      }
      Lux::_close(fd);
      if (is_failed) { return false; }
    }
    flush_counter_++;
    return true;
  }

  bool BatchIndexer::get_records(TLA &tla, int *fds, off_t *offs, uint32_t idx, uint32_t num)
  {
    int fd = fds[idx];
    uint32_t off = offs[idx];

    for (uint32_t i = 0; i < num; ++i) {
      size_t term_size;
      if (!_pread(fd, &term_size, sizeof(size_t), off)) {
        return NULL;
      }
      off += sizeof(size_t);
      char *term = new char[term_size+1];
      if (!_pread(fd, term, sizeof(char) * term_size, off)) {
        return NULL;
      }
      term[term_size] = '\0';
      off += term_size;

      size_t list_size;
      if (!_pread(fd, &list_size, sizeof(size_t), off)) {
        return NULL;
      }
      off += sizeof(size_t);
      char *list = new char[list_size];
      if (!_pread(fd, list, sizeof(char) * list_size, off)) {
        return NULL;
      }
      off += list_size;

      tla.push_back(term_list(term, term_size, list, list_size, idx));
    }

    offs[idx] = off;
    return true;
  }

}
