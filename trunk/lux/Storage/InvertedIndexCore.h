#ifndef LUX_INVERTED_INDEX_CORE_H
#define LUX_INVERTED_INDEX_CORE_H

#include "lux/luxconfig.h"
#include "lux/lux.h"
#include "Posting.h"
#include "lux/StorageEngine/StorageEngine.h"
#include "lux/Document/DocumentDefinition.h"
#include "lux/Document/Document.h"
#include "lux/Lexer/Term.h"
#include "lux/scoped_ptr.h"

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {

  const std::string INDEX_SUFFIX = ".idx";
  
  typedef std::map< std::string, std::string > StrMap;
  typedef StrMap::iterator SMIterator;
  typedef std::set< std::string > StrSet;
  typedef StrSet::iterator SSIterator;
  typedef std::pair< SSIterator, bool > StrSetResult;

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
    PostingList pbuffer;
  };
  typedef std::set< TermBuffer, less_term<TermBuffer> > TermBufferSet;
  typedef TermBufferSet::iterator TBIterator;

  struct IndexEngine {
    IndexEngine(const std::string &index_to_,
                const std::string &index_file_, db_flags_t open_params_)
    : index_to(index_to_),
      engine(new LuxBtreeStorage(index_file_, open_params_))
    {}
    IndexEngine(const std::string &index_to_)
    : index_to(index_to_)
    {}

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
    IndexEngineSet(std::string storage_dir, db_flags_t open_params,
                   DocumentDefinition &doc_def)
    {
      doc_def.init_iter();
      while (doc_def.has_next()) {
        FieldDefinition fd = doc_def.get_next();
        if (!fd.is_indexed()) { continue; }

        std::string index_to = fd.get_index_to();
        StrSetResult res = initialized_set_.insert(index_to);
        if (!res.second) { continue; }

        std::string index_file = index_to + INDEX_SUFFIX;
        index_file.insert(0, storage_dir + "/");
        ie_set_.insert(IndexEngine(index_to, index_file, open_params));
      }
    }
  
    void flush(const std::string &index_to, const TermBufferSet &term_buffer_set)
    {
      // select engine
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to));

      //ie_itr->engine
      TBIterator tb_itr = term_buffer_set.begin(); 
      TBIterator tb_itr_end = term_buffer_set.end(); 
      while (tb_itr != tb_itr_end) {

        Term t = tb_itr->term;
        char *intarray;
        int intarray_size;
        intarray = get_intarray_from(const_cast<PostingList &>(tb_itr->pbuffer),
                                     &intarray_size);
        
        LuxDataUnit key(const_cast<char *>(t.text_.c_str()), t.text_.size());
        LuxDataUnit val(intarray, intarray_size);
        ie_itr->engine->append(key, val);

        delete [] intarray;
        ++tb_itr;
      }
    }

    bool exist(const std::string &index_to)
    {
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to));
      if (ie_itr == ie_set_.end()) {
        return false;
      }
      return true;
    }

    IndexEngine get(const std::string &index_to)
    {
      IEIterator ie_itr = ie_set_.find(IndexEngine(index_to));
      if (ie_itr == ie_set_.end()) {
        std::cerr << "fatal error: unexisted index " << index_to << std::endl;
      }
      return *ie_itr;
    }
    
  private:
    IESet ie_set_;
    StrSet initialized_set_;
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
    IndexBufferSet(DocumentDefinition &doc_def)
    : curr_buffer_size_(0)
    {
      doc_def.init_iter();
      while (doc_def.has_next()) {
        FieldDefinition fd = doc_def.get_next();
        if (!fd.is_indexed()) { continue; }

        IndexBuffer index_buffer(fd.get_index_to());
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

        TBIterator tb_itr2 = ib_itr->term_buffer_set.find(*tb_itr);
        if (tb_itr2 == ib_itr->term_buffer_set.end()) {
          const_cast<TermBufferSet&>(ib_itr->term_buffer_set).insert(*tb_itr);
          curr_buffer_size_ += tb_itr->pbuffer.size() * sizeof(uint_t);
          curr_buffer_size_ += tb_itr->term.text_.size();
        } else {
          PLIterator itr = const_cast<PostingList&>(tb_itr->pbuffer).begin();
          PLIterator itr_end = const_cast<PostingList&>(tb_itr->pbuffer).end();
          for (; itr != itr_end; ++itr) {
            const_cast<PostingList&>(tb_itr2->pbuffer).push_back(*itr);
          }
          curr_buffer_size_ += tb_itr->pbuffer.size() * sizeof(uint_t);
        }
      }
    }

    // it's not exact memory use for index buffer
    uint_t get_buffer_size(void)
    {
      return curr_buffer_size_;
    }

    void print()
    {
      IBIterator ib_itr = ib_set_.begin(); 
      while (ib_itr != ib_set_.end()) {
        
        std::cout << "INDEX_TO: " << ib_itr->index_to << std::endl;
        TBIterator tb_itr = ib_itr->term_buffer_set.begin(); 
        while (tb_itr != ib_itr->term_buffer_set.end()) {
          std::cout << "term [" << tb_itr->term.text_ << "] "
                    << std::endl;
          PLIterator pl_itr = const_cast<PostingList&>(tb_itr->pbuffer).begin();
          std::cout << "posting list : ";
          for (; pl_itr != tb_itr->pbuffer.end(); ++pl_itr) {
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
    uint_t curr_buffer_size_; 
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
     * constructor: initialize each IndexData object with DocumentDifinition
     */
    IndexDataSet(DocumentDefinition &doc_def)
    {
      doc_def.init_iter();
      while (doc_def.has_next()) {
        FieldDefinition fd = doc_def.get_next();
        if (!fd.is_indexed()) { continue; }

        std::string index_to = fd.get_index_to();
        std::string field_name = fd.get_field_name();
        field_to_index_.insert(make_pair(field_name, index_to));

        IndexData index_data(index_to);
        index_data.is_exact = fd.is_exact();
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
        SMIterator sm_itr = field_to_index_.find(f->get_name());
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
    StrMap field_to_index_;
  };

}

#endif
