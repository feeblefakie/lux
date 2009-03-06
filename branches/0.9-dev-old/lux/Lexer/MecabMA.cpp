#include "MecabMA.h"
#include "lux/LangUtil/UTF8Util.h"

namespace Lux {

  MecabMA::MecabMA() 
  :tagger_(MeCab::createTagger(0, NULL)),
   lang_util_(new Lux::UTF8Util()) 
  {
    tokens_.reserve(128);
  }

  MecabMA::~MecabMA()
  {
  }

  bool MecabMA::tokenize(const char *str)
  {
    unsigned int offset = 0;
    // should empty tokens_
    const MeCab::Node *node_ = tagger_->parseToNode(str);
    for (; node_; node_ = node_->next) {
      if (node_->stat == MECAB_BOS_NODE ||
          node_->stat == MECAB_EOS_NODE) {
        continue; 
      }
      std::string token(node_->surface, node_->length);
      unsigned int length = lang_util_->get_length_of(token);
      // mecab ignores spaces in default 
      // but they must be counted as offset from the beginning
      int head_space_len =  node_->rlength - node_->length;
      offset += head_space_len > 0 ? head_space_len : 0;

      tokens_.push_back(Term(token, length, offset));
      offset += length;
    }
    tokens_iterator_ = tokens_.begin();

    return true; 
  }

  bool MecabMA::tokenize(const std::string &str)
  {
    return this->tokenize(str.c_str());
  }

  bool MecabMA::has_next()
  {
    if (tokens_iterator_ == tokens_.end()) {
      return false;
    }
    return true;
  }

  Term MecabMA::get_next_token()
  {
    return *tokens_iterator_++;
  }

  void MecabMA::init_iter()
  {
    tokens_iterator_ = tokens_.begin();        
  }

  void MecabMA::clear()
  {
    tokens_.clear();
  }
}
