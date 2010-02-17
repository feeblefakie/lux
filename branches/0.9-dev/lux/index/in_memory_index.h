#ifndef LUX_INDEX_INMEMORYINDEX_H
#define LUX_INDEX_INMEMORYINDEX_H

#include "lux/lux.h"
#include "lux/util.h"
#include "lux/index/vbc.h"
#include "lux/index/normalizer.h"
#include "lux/document/document_definition.pb.h"
#include "lux/document/document.h"
#include "lux/lexer/term.h"
#include "lux/lexer/lexer.h"
#include "lux/scoped_ptr.h"

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

//#include <boost/pool/pool_alloc.hpp>

namespace Lux {
namespace TMP { // temporary namespace

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

  typedef std::set< TermBuffer, less_term<TermBuffer> > TermBufferSet;
  typedef TermBufferSet::iterator TBIterator;


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
    
    void init_iter(void)
    {
      ib_itr_ = ib_set_.begin(); 
    }

    bool has_next(void)
    {
      if (ib_itr_ == ib_set_.end()) {
        return false;
      }
      return true;
    }

    std::string get_next_index_to(void)
    {
      return (ib_itr_++)->index_to;
    }

    TermBufferSet get_term_buffer_set(std::string &index_to)
    {
      IBIterator ib_itr = ib_set_.find(IndexBuffer(index_to));
      return const_cast<TermBufferSet&>(ib_itr->term_buffer_set);
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

  private:
    IBSet ib_set_;
    IBIterator ib_itr_;
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

  /**
   * InMemoryIndex
   */
  class InMemoryIndex {
  public:
    InMemoryIndex(Config::Document &doc_config)
    : lexer_(new LuxLexer),
      index_data_set_(new IndexDataSet(doc_config)),
      index_buffer_set_(new IndexBufferSet(doc_config))
    {
      //index_data_set_.reset(new IndexDataSet(doc_config));
      //index_buffer_set_.reset(new IndexBufferSet(doc_config));
    }

    ~InMemoryIndex()
    {
    }

    bool add(const Document *doc)
    {
      return add(*doc);
    }

    bool add(const Document &doc)
    {
      index_data_set_->divide(doc);

      index_data_set_->init_iter();
      while (index_data_set_->has_next()) {
        TermBufferSet tb_set;
        IndexData index_data = index_data_set_->get_next();

        Normalizer::process(index_data.data);
        
        /* for exact match */
        if (index_data.is_exact) {
          TermBuffer tb(Term(index_data.data));
          tb.buffer.push_back(doc.get_id());
          // no term frequency and offsets are stored
          tb_set.insert(tb);

        /* for partial match */
        } else {
          lexer_->tokenize(index_data.data);
          lexer_->init_iter();
          while (lexer_->has_next()) {
            Term term = lexer_->get_next_token();
            TermBuffer tb(term);
            TBIterator tb_itr = tb_set.find(tb);

            if (tb_itr != tb_set.end()) {
              // append another offset and increment term frequency
              const_cast<pos_list_t&>(tb_itr->buffer).push_back(term.offset_);
              ++const_cast<pos_list_t&>(tb_itr->buffer).at(1);
            } else {
              // create term buffer with initial values
              tb.buffer.push_back(doc.get_id());
              tb.buffer.push_back(1); // term frequency
              tb.buffer.push_back(term.offset_);
              tb_set.insert(tb);
            }
          }
          lexer_->clear();
        }
        // update index buffer
        index_buffer_set_->merge(index_data.index_to, tb_set);
      }
      index_data_set_->clear_data();
      return true;
    }

    void init_iter(void)
    {
      index_buffer_set_->init_iter();
    }

    bool has_next(void)
    {
      return index_buffer_set_->has_next();
    }

    std::string get_next_index_to(void)
    {
      return index_buffer_set_->get_next_index_to();
    }

    TermBufferSet get_term_buffer_set(std::string &index_to)
    {
      return index_buffer_set_->get_term_buffer_set(index_to);
    }

    uint32_t get_buffer_size(void)
    {
      return index_buffer_set_->get_buffer_size();
    }

  private:
    scoped_ptr<LuxLexer> lexer_;
    scoped_ptr<IndexBufferSet> index_buffer_set_;
    scoped_ptr<IndexDataSet> index_data_set_;

  };

}
}

#endif
