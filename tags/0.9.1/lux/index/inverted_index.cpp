#include "inverted_index.h"
#include "boolean_merger.h"
#include "normalizer.h"
#include "lux/query/term_query.h"
#include "lux/query/phrase_query.h"
#include "lux/query/boolean_query.h"
#include <iostream>

namespace Lux {

  InvertedIndex::InvertedIndex(Config::Document &doc_config)
  : lexer_(new LuxLexer),
    index_engine_set_(IndexEngineSet(doc_config)),
    doc_config_(doc_config)
  {}

  InvertedIndex::~InvertedIndex()
  {}

  bool InvertedIndex::open(std::string storage_dir, db_flags_t open_params)
  {
    if (open_params ^ DB_RDONLY) {
      index_data_set_.reset(new IndexDataSet(doc_config_));
      index_buffer_set_.reset(new IndexBufferSet(doc_config_));
    }
    if (!index_engine_set_.open(storage_dir, open_params)) {
      return false; 
    }
    return true;
  }

  bool InvertedIndex::close(void)
  {
    // nothing to be done
    if (!index_engine_set_.close()) {
      return false;
    }
    return true;
  }

  bool InvertedIndex::add(const Document *doc)
  {
    return add(*doc);
  }

  bool InvertedIndex::add(const Document &doc)
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

  void InvertedIndex::flush(void)
  {
    // flush index_buffer to index_engine
    index_buffer_set_->flush(index_engine_set_);
  }

  uint32_t InvertedIndex::get_buffer_size(void)
  {
    return index_buffer_set_->get_buffer_size();
  }

  void InvertedIndex::print_buffer(void)
  {
    index_buffer_set_->print();
  }

  void InvertedIndex::set_sys_cond(sys_cond_t sys_cond)
  {
    sys_cond_ = sys_cond;
  }

  IndexResultSet InvertedIndex::search(Query *query)
  {
    IndexResultSet rs = query->search(this);

    return rs;
  }

  IndexResultSet InvertedIndex::search_term(TermQuery *qterm)
  {
    Posting p = get_term_pos_(qterm);
    IndexResultSet rs = pos_to_rs_(p);

    return rs;
  }

  Posting InvertedIndex::get_term_pos_(TermQuery *qterm)
  {
    Posting p;
    LuxDataUnit *u = get_term_unit_(qterm);
    p.skipped = (sys_cond_ & RES_WO_POS) ? true : false;
    p.list = vb_decode((uint8_t *) u->get_data(), u->get_size(), p.skipped); 
    delete u;

    return p;
  }

  LuxDataUnit *InvertedIndex::get_term_unit_(TermQuery *qterm)
  {
    Term term = qterm->get_term();
    Normalizer::process(term.text_);
    char *key_term = const_cast<char *>(term.text_.c_str());
    LuxDataUnit key(key_term, strlen(key_term));

    if (!index_engine_set_.exist(qterm->get_index())) {
        return NULL; // no index found
    }

    LuxDataUnit *val = new LuxDataUnit;
    IndexEngine ie = index_engine_set_.get(qterm->get_index());
    if (!ie.engine || !ie.engine->get(key, *val)) {
        delete val;
        return NULL; // no result found
    }

    return val;
  }

  IndexResultSet InvertedIndex::search_phrase(PhraseQuery *qphrase)
  {
    Posting p = get_phrase_pos_(qphrase);
    IndexResultSet rs = pos_to_rs_(p);

    return rs;
  }

  Posting InvertedIndex::get_phrase_pos_(PhraseQuery *qphrase)
  {
    uint32_t idx = 0;
    std::vector<LuxDataUnit *> units;
    std::vector<uint32_t> q_offs;
    std::vector<PostingInf> p_infs;
    std::vector<PostingInf>::iterator itr;
    Posting p;

    // [NOTICE] allocate lexer in every call for thread safety
    LuxLexer lexer;
    lexer.tokenize(qphrase->get_phrase());
    while (lexer.has_next()) {
      Term t = lexer.get_next_token();
      TermQuery *tq = TermQuery::create(t, qphrase->get_index());
      LuxDataUnit *_u = get_term_unit_(tq);
      delete tq;
      if (_u == NULL) { return p; }

      units.push_back(_u);
      q_offs.push_back(t.offset_);
      p_infs.push_back(PostingInf(_u->get_size(), idx++));
    }
    // sort plist index by size to merge from the smallest
    sort(p_infs.begin(), p_infs.end(), sort_by_size());

    if (p_infs.size() == 1) {
      LuxDataUnit *u = units[0];
      p.skipped = (sys_cond_ & RES_WO_POS) ? true : false;
      p.list = vb_decode((uint8_t *) u->get_data(), u->get_size(), p.skipped);
    } else if (p_infs.size() > 1) {
      p.skipped = false;
      itr = p_infs.begin();
      uint32_t p_idx = (itr++)->idx;
      uint32_t n_idx = (itr++)->idx;
      int q_diff = q_offs[n_idx] - q_offs[p_idx];
      
      // for 2 tokens
      if (q_diff < 0) {
        p.list = _intersect(units[n_idx], units[p_idx], -q_diff);
        p_idx = n_idx;
      } else {
        p.list = _intersect(units[p_idx], units[n_idx], q_diff);
      }

      // for more than 2 tokens
      for (; itr != p_infs.end(); ++itr) {
        n_idx = itr->idx;
        q_diff = q_offs.at(n_idx) - q_offs.at(p_idx);

        if (q_diff < 0) {
          p.list = _intersect(units[n_idx], p.list, -q_diff);
          p_idx = n_idx;
        } else {
          p.list = _intersect(p.list, units[n_idx], q_diff);
        }
      }
    }
    for (int i = 0; i < units.size(); ++i) { delete units[i]; }
    
    return p;
  }

  pos_list_t InvertedIndex::_intersect(LuxDataUnit *u1, LuxDataUnit *u2, int diff)
  {
    return intersect((uint8_t *) u1->get_data(), u1->get_size(),
                     (uint8_t *) u2->get_data(), u2->get_size(), diff);
  }

  pos_list_t InvertedIndex::_intersect(LuxDataUnit *u, pos_list_t &p, int diff)
  {
    return intersect((uint8_t *) u->get_data(), u->get_size(), p, diff);
  }

  pos_list_t InvertedIndex::_intersect(pos_list_t &p, LuxDataUnit *u, int diff)
  {
    return intersect(p, (uint8_t *) u->get_data(), u->get_size(), diff);
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

  IndexResultSet InvertedIndex::pos_to_rs_(Posting &p)
  {
    pos_list_itr itr = p.list.begin();
    pos_list_itr itr_end = p.list.end();
    unsigned int plist_size = p.list.size();

    IndexResultSet rs;
    rs.reserve(plist_size);
    for (; itr != itr_end; ++itr) {
      // [TODO] it's temporary
      // [FIXME] wrong calculation for single term
      // reading invalid memory
      score_t score = 100000 * *(itr+1) / plist_size;
      rs.push_back(IndexResult(*itr, score));

      if (!p.skipped) {
        itr += *(++itr);
      }
    }
    return rs;
  }

}
