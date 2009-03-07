#ifndef LUX_INDEX_INVERTEDINDEXCORE_H
#define LUX_INDEX_INVERTEDINDEXCORE_H

#include "lux/lux.h"
#include "posting.h"
#include "vbc.h"
#include "lux/storage/storage_engine.h"
#include "lux/document/document_definition.pb.h"
#include "lux/document/document.h"
#include "lux/lexer/term.h"
#include "lux/scoped_ptr.h"

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

#include <tr1/unordered_set>
//#include <boost/pool/pool_alloc.hpp>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  const std::string INDEX_SUFFIX = ".idx";
  
  typedef std::map< std::string, std::string > str_map;
  typedef str_map::iterator str_map_itr;
  typedef std::set< std::string > str_set;
  typedef str_set::iterator str_set_itr;
  typedef std::pair< str_set_itr, bool > str_set_result;

  template <class T>    
  struct less_index_to : public std::binary_function <T, T, bool> {
    bool operator() (const T& t1, const T& t2) const {
      return t1.index_to < t2.index_to;
    }
  };

  template <class T>    
  struct less_term : public std::binary_function <T, T, bool> {
    bool operator() (const T& t1, const T& t2) const {
      return t1.term.text_ < t2.term.text_;
    }
  };

  struct TermBuffer {
    TermBuffer(Term term_)
    : term(term_)
    {}
    Term term;
    pos_list_t buffer;
  };

  struct term_buffer_hash {
    std::tr1::hash<std::string> hs;
    size_t operator()(const TermBuffer& x) const {
      return static_cast<size_t>(hs(x.term.text_));
    }
  };

  struct term_buffer_equal {
    bool operator()(const TermBuffer& x, const TermBuffer& y) const {
      return x.term.text_ == y.term.text_;
    }
  };

  typedef std::tr1::unordered_set
    < TermBuffer,
      term_buffer_hash,
      term_buffer_equal
      //boost::pool_allocator<TermBuffer>
    > TermBufferSet;

  //typedef std::set< TermBuffer, less_term<TermBuffer> > TermBufferSet;
  typedef TermBufferSet::iterator TBIterator;

  struct IndexEngine {
    IndexEngine(const std::string &index_to_)
    : index_to(index_to_),
      engine(new LuxBtreeStorage)
    {}

    IndexEngine(const std::string &index_to_, bool is_for_comparison)
    : index_to(index_to_)
    {}

    bool open(const std::string &index_file_, db_flags_t open_params_)
    {
      return engine->open(index_file_.c_str(), open_params_);
    }

    bool close(void)
    {
      return engine->close();
    }

    std::string index_to;
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<LuxBtreeStorage> engine;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<LuxBtreeStorage> engine;
#endif
  };

  typedef std::set< IndexEngine, less_index_to<IndexEngine> > IESet;
  typedef IESet::iterator IEIterator;

  /**
   * class IndexEngineSet
   */
  class IndexEngineSet {
  public:
    IndexEngineSet(Config::Document &doc_config)
    : doc_config_(doc_config)
    {}

    bool open(std::string storage_dir, db_flags_t open_params)
    {
      for (int i = 0; i < doc_config_.field_size(); i++) {
        const Lux::Config::Field &field = doc_config_.field(i);

        if (!field.index().indexing()) { continue; }

        std::string index_to = field.index().index_to();
        str_set_result res = initialized_set_.insert(index_to);
        if (!res.second) { continue; }

        std::string index_file = index_to + INDEX_SUFFIX;
        index_file.insert(0, storage_dir + "/");
        IndexEngine ie(index_to);
        if (!ie.open(index_file, open_params)) {
          return false;
        }
        ie_set_.insert(ie);
      }
      return true;
    }

    bool close(void)
    {
      // all the engines will be closed automatically in destructor,
      // so this method does nothing for now.
      return true;
    }

    void flush(const std::string &index_to, const TermBufferSet &term_buffer_set)
    {
      // select engine
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to, true));

      //ie_itr->engine
      TBIterator tb_itr = const_cast<TermBufferSet&>(term_buffer_set).begin();
      TBIterator tb_itr_end = const_cast<TermBufferSet&>(term_buffer_set).end(); 
      while (tb_itr != tb_itr_end) {

        Term t = tb_itr->term;
        std::vector<uint8_t> bs = vb_encode(const_cast<pos_list_t&>(tb_itr->buffer));
        vb_encode_num(0, bs); // mark
        
        LuxDataUnit key(const_cast<char *>(t.text_.c_str()), t.text_.size());
        //LuxDataUnit val(intarray, intarray_size);
        LuxDataUnit val((uint8_t *) &(bs[0]), bs.size());
        if (!ie_itr->engine->append(key, val)) {
          error_log("append to inverted index failed.");
          throw std::runtime_error("append failed.");
        }

        // clear and minimize idiom
        pos_list_t().swap(const_cast<pos_list_t&>(tb_itr->buffer));

        //delete [] intarray;
        ++tb_itr;
      }
    }

    bool exist(const std::string &index_to)
    {
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to, true));
      if (ie_itr == ie_set_.end()) {
        return false;
      }
      return true;
    }

    IndexEngine get(const std::string &index_to)
    {
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to, true));
      if (ie_itr == ie_set_.end()) {
        std::cerr << "fatal error: unexisted index " << index_to << std::endl;
      }
      return *ie_itr;
    }
    
  private:
    IESet ie_set_;
    str_set initialized_set_;
    Config::Document doc_config_;
  };

  /**
   * IndexBuffer
   */
  struct IndexBuffer {
    IndexBuffer(std::string index_to_)
    : index_to(index_to_)
    {}
    std::string index_to;
    TermBufferSet term_buffer_set;
  };
  typedef std::set< IndexBuffer, less_index_to<IndexBuffer> > IBSet;
  typedef IBSet::iterator IBIterator;

  /**
   * class IndexBufferSet
   */
  class IndexBufferSet {

  public:
    IndexBufferSet(Config::Document &doc_config)
    : curr_buffer_size_(0)
    {
      for (int i = 0; i < doc_config.field_size(); i++) {
        const Lux::Config::Field &field = doc_config.field(i);

        if (!field.index().indexing()) { continue; }

        IndexBuffer index_buffer(field.index().index_to());
        // IndexData with the same index_to will fail,
        // so no duplicate checking
        ib_set_.insert(index_buffer);
      }
    }

    // merge with TermBufferSet for index_to
    void merge(std::string &index_to, TermBufferSet &term_buffer_set)
    {
      IBIterator ib_itr = ib_set_.find(IndexBuffer(index_to));
      TBIterator tb_itr_end = term_buffer_set.end();
      for (TBIterator tb_itr = term_buffer_set.begin();
           tb_itr != tb_itr_end; ++tb_itr) {

        TBIterator tb_itr2 = const_cast<TermBufferSet&>(ib_itr->term_buffer_set).find(*tb_itr);
        if (tb_itr2 == ib_itr->term_buffer_set.end()) {
          const_cast<TermBufferSet&>(ib_itr->term_buffer_set).insert(*tb_itr);
          curr_buffer_size_ += tb_itr->buffer.size() * sizeof(uint32_t);
          curr_buffer_size_ += tb_itr->term.text_.size();
        } else {
          pos_list_itr itr = const_cast<pos_list_t&>(tb_itr->buffer).begin();
          pos_list_itr itr_end = const_cast<pos_list_t&>(tb_itr->buffer).end();
          for (; itr != itr_end; ++itr) {
            const_cast<pos_list_t&>(tb_itr2->buffer).push_back(*itr);
          }
          curr_buffer_size_ += tb_itr->buffer.size() * sizeof(uint32_t);
        }
      }
    }

    // it's not exact memory use for index buffer
    uint32_t get_buffer_size(void)
    {
      return curr_buffer_size_;
    }

    void print()
    {
      IBIterator ib_itr = ib_set_.begin(); 
      while (ib_itr != ib_set_.end()) {
        
        std::cout << "INDEX_TO: " << ib_itr->index_to << std::endl;
        TBIterator tb_itr = const_cast<TermBufferSet&>(ib_itr->term_buffer_set).begin(); 
        while (tb_itr != ib_itr->term_buffer_set.end()) {
          std::cout << "term [" << tb_itr->term.text_ << "] "
                    << std::endl;
          pos_list_itr pl_itr = const_cast<pos_list_t&>(tb_itr->buffer).begin();
          std::cout << "posting list : ";
          for (; pl_itr != tb_itr->buffer.end(); ++pl_itr) {
            std::cout << *pl_itr << ",";
          }
          std::cout << std::endl;
          ++tb_itr;
        }
        ++ib_itr;
      }
    }

    void flush(IndexEngineSet &index_engine_set)
    {
      IBIterator ib_itr = ib_set_.begin(); 
      while (ib_itr != ib_set_.end()) {

        index_engine_set.flush(ib_itr->index_to, ib_itr->term_buffer_set);
        const_cast<TermBufferSet&>(ib_itr->term_buffer_set).clear();
        ++ib_itr;
      }
      curr_buffer_size_ = 0;
    }

  private:
    IBSet ib_set_;
    // used as a threshold for flushing,
    // so it's not exact memory use for index buffer.
    uint32_t curr_buffer_size_; 
  };


  /**
   * class IndexData
   */
  struct IndexData {
    IndexData(std::string index_to_)
    : index_to(index_to_)
    {}
    std::string index_to;
    std::string data;
    bool is_exact;
  };
  typedef std::set< IndexData, less_index_to<IndexData> > IDSet;
  typedef IDSet::iterator IDIterator;

  /**
   * IndexDataSet
   */
  class IndexDataSet {
  public:
    /**
     * constructor: initialize each IndexData object with document definition
     */
    IndexDataSet(Config::Document &doc_config)
    {
      for (int i = 0; i < doc_config.field_size(); i++) {
        const Lux::Config::Field &field = doc_config.field(i);

        if (!field.index().indexing()) { continue; }

        field_to_index_.insert(
            make_pair(field.name(), field.index().index_to())
          );

        IndexData index_data(field.index().index_to());
        index_data.is_exact = field.index().has_exact();
        // IndexData with the same index_to will fail
        id_set_.insert(index_data);
      }
    }

    /**
     *  divide the fields' data of a Document into each index
     */
    void divide(const Document &doc)
    {
      doc.init_iter();
      while (doc.has_next()) {
        const Lux::Field *f = doc.get_next();
        str_map_itr sm_itr = field_to_index_.find(f->get_name());
        // this field is not indexed
        if (sm_itr == field_to_index_.end()) { continue; }

        IDIterator id_itr = id_set_.find(IndexData(sm_itr->second));
        if (id_itr->data.empty()) {
          const_cast<std::string&>(id_itr->data) = f->get_value(); 
        } else {
          // data for the same index is appended
          const_cast<std::string&>(id_itr->data) += "" + f->get_value(); 
        }
      }
    }

    void init_iter() { id_itr_ = id_set_.begin(); }
    bool has_next() { return id_itr_ != id_set_.end(); }
    IndexData get_next() { return *id_itr_++; }
    /**
     * clearing the data specified by iterator only.
     */
    void clear_current_data() { const_cast<std::string&>(id_itr_->data) = ""; }
    void clear_data()
    {
      for (id_itr_ = id_set_.begin(); id_itr_ != id_set_.end(); ++id_itr_) {
        const_cast<std::string&>(id_itr_->data) = "";
      }
    }

  private:
    IDSet id_set_;
    IDIterator id_itr_;
    str_map field_to_index_;
  };

}

#endif
