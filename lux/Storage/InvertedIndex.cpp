#include "InvertedIndex.h"
#include "BooleanMerger.h"
#include "Normalizer.h"
#include "lux/Query/TermQuery.h"
#include "lux/Query/PhraseQuery.h"
#include "lux/Query/BooleanQuery.h"
#include <iostream>

namespace Lux {

  InvertedIndex::InvertedIndex(std::string storage_dir, db_flags_t open_params, 
                               DocumentDefinition &doc_def)
  : lexer_(new LuxLexer),
    index_engine_set_(IndexEngineSet(storage_dir, open_params, doc_def)),
    index_data_set_(open_params ^ DB_RDONLY ? new IndexDataSet(doc_def) : NULL),
    index_buffer_set_(open_params ^ DB_RDONLY ? new IndexBufferSet(doc_def) : NULL)
  {}

  InvertedIndex::~InvertedIndex() {}

  void InvertedIndex::add(const Document *doc)
  {
    add(*doc);
  }

  void InvertedIndex::add(const Document &doc)
  {
    index_data_set_->divide(doc);

    index_data_set_->init_iter();
    while (index_data_set_->has_next()) {
      TermBufferSet term_buffer_set;
      IndexData index_data = index_data_set_->get_next();

      //std::cout << index_data.data << std::endl;
      Normalizer::process(index_data.data);
      
      /* for exact match */
      if (index_data.is_exact) {
        TermBuffer term_buffer(Term(index_data.data));
        term_buffer.pbuffer.push_back(doc.get_internal_id());
        // no term frequency and offsets are stored
        term_buffer_set.insert(term_buffer);

      /* for partial match */
      } else {

        lexer_->tokenize(index_data.data);

        lexer_->init_iter();
        while (lexer_->has_next()) {
          Term term = lexer_->get_next_token();
          TermBuffer term_buffer(term);
          TBIterator tb_itr = term_buffer_set.find(term_buffer);

          if (tb_itr != term_buffer_set.end()) {
            // append another offset and increment term frequency
            const_cast<PostingList&>(tb_itr->pbuffer).push_back(term.offset_);
            ++const_cast<PostingList&>(tb_itr->pbuffer).at(1);
          } else {
            // create term buffer with initial values
            term_buffer.pbuffer.push_back(doc.get_internal_id());
            term_buffer.pbuffer.push_back(1); // term frequency
            term_buffer.pbuffer.push_back(term.offset_);
            term_buffer_set.insert(term_buffer);
          }
        }
        lexer_->clear();
      }

      // update index buffer
      index_buffer_set_->merge(index_data.index_to, term_buffer_set);
    }
    index_data_set_->clear_data();
  }

  void InvertedIndex::flush(void)
  {
    // flush index_buffer to index_engine
    index_buffer_set_->flush(index_engine_set_);
  }

  uint_t InvertedIndex::get_buffer_size(void)
  {
    return index_buffer_set_->get_buffer_size();
  }

  void InvertedIndex::print_buffer(void)
  {
    index_buffer_set_->print();
  }

  IndexResultSet InvertedIndex::search(Query *query)
  {
    IndexResultSet rs = query->search(this);

    return rs;
  }

  // 各Queryクラスから呼ばれる
  IndexResultSet InvertedIndex::search_term(TermQuery *qterm)
  {
    PostingList plist = get_term_plist_(qterm);

    // calc scores
    // convert from plist to IndexResultSet;
    IndexResultSet rs = plist_to_rs_(plist);
    return rs;
  }

  PostingList InvertedIndex::get_term_plist_(TermQuery *qterm)
  {
    Term term = qterm->get_term();
    Normalizer::process(term.text_);

    char *key_term = const_cast<char *>(term.text_.c_str());
    LuxDataUnit key(key_term, strlen(key_term)), val;

    if (!index_engine_set_.exist(qterm->get_index())) {
        // no index found
        PostingList plist;
        return plist;
    }

    IndexEngine ie = index_engine_set_.get(qterm->get_index());
    if (!ie.engine || !ie.engine->get(key, val)) {
        // no result found
        PostingList plist;
        return plist;
    }

    return get_plist_from(static_cast<char *>(val.get_data()), val.get_size());
  }

  IndexResultSet InvertedIndex::search_phrase(PhraseQuery *qphrase)
  {
    PostingList plist = get_phrase_plist_(qphrase);

    // convert from plist to IndexResultSet;
    IndexResultSet rs = plist_to_rs_(plist);
    //std::cout << "IndexResultSet size: " << rs.size() << std::endl;
    return rs;
  }

  PostingList InvertedIndex::get_phrase_plist_(PhraseQuery *qphrase)
  {
    lexer_->clear();
    lexer_->tokenize(qphrase->get_phrase());
    PLArray plist_array;
    unsigned int q_off_from_head = 0;
    unsigned int idx = 0;
    std::vector<unsigned int> q_off_arr;
    PLA_inf pla_inf;

    while (lexer_->has_next()) {
      Term t = lexer_->get_next_token();
      //std::cout << "term: " << t.text_ << std::endl;
      //std::cout << "term_len: " << t.length_ << std::endl;
      TermQuery *tq = TermQuery::create(t, qphrase->get_index());
      PostingList plist = get_term_plist_(tq);
      delete tq;
      //std::cout << "size: " << plist.size() << std::endl;

      plist_array.push_back(plist);
      q_off_arr.push_back(t.offset_);
      pla_inf.push_back(PLA_elem_inf(plist.size(), idx++));
    }

    if (pla_inf.empty()) { std::cerr << "no match" << std::endl; }

    // sort plist index by size to merge from the smallest
    sort(pla_inf.begin(), pla_inf.end(), sort_by_size());

    PostingList plist; 
    if (pla_inf.size() == 1) {
      plist = plist_array.at(0);
    } else {
      PLAIIterator itr = pla_inf.begin();
      PLAIIterator itr_end = pla_inf.end();
      unsigned int p_idx = itr->pla_idx;
      plist = plist_array.at(p_idx);
      for (++itr; itr != itr_end; ++itr) {
        unsigned int n_idx = itr->pla_idx;
        PostingList n_plist = plist_array.at(n_idx);
        int q_off_diff = q_off_arr.at(n_idx) - q_off_arr.at(p_idx);

        if (q_off_diff < 0) {
          plist = merge_plist(n_plist, plist, -q_off_diff);
          p_idx = n_idx;
        } else {
          plist = merge_plist(plist, n_plist, q_off_diff);
        }
      }
    }
    return plist;
  }

  IndexResultSet InvertedIndex::search_boolean(BooleanQuery *qboolean)
  {
    //std::cout << "is_composite: " << qboolean->is_composite() << std::endl;
    IndexResultSet rs;
    int q_idx = 0;
    qboolean->init_iter();
    while (qboolean->has_next()) {
      const QuerySet qset = qboolean->get_next();

      //std::cout << "query: " << qset.query->to_str() << std::endl;
      // first query
      if (q_idx == 0) {
        rs = qset.query->search(this);
        ++q_idx;
        continue;
      }
      
      IndexResultSet rs1 = qset.query->search(this);

      switch (qset.op_type) {
      case OP_AND:
        rs = BooleanMerger::AND(rs, rs1);
        break;
      case OP_OR:
        rs = BooleanMerger::OR(rs, rs1);
        break; 
      case OP_NOT:
        rs = BooleanMerger::NOT(rs, rs1);
        break; 
      }

      //std::cout << "OP: " << qset.op_type << std::endl;
      ++q_idx;
    }

    //std::cout << "IndexResultSet size: " << rs.size() << std::endl;
    return rs;
  }

  IndexResultSet InvertedIndex::plist_to_rs_(PostingList &plist)
  {
    PLIterator itr = plist.begin();
    PLIterator itr_end = plist.end();
    unsigned int plist_size = plist.size();

    IndexResultSet rs;
    for (; itr != itr_end; ++itr) {
      // [TODO] it's temporary
      unsigned int score = 100000 * *(itr+1) / plist_size;
      rs.push_back(IndexResult(*itr, score));
      itr += *(++itr);
    }
    return rs;
  }

}
