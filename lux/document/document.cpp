#include "document.h"

namespace Lux {

  Document::Document(char *id)
  : external_id_(id)
  {}

  Document::Document(std::string &id)
  : external_id_(id)
  {}

  Document::Document(const Document &doc)
  : external_id_(doc.external_id_), fields_(doc.fields_)
  {
    fields_iterator_ = fields_.begin();
  }

  Document &Document::operator=(const Document &rhs)
  {
    external_id_ = rhs.external_id_;
    fields_ = rhs.fields_;
    fields_iterator_ = fields_.begin();
    return *this;
  }

  Document::~Document(void)
  {}

  void Document::add(const Field *field)
  {
    fields_.push_back(SPF(field));
    fields_iterator_ = fields_.begin();
  }

  bool Document::has_next(void)
  {
    return static_cast<const Document&>(*this).has_next();
  }

  bool Document::has_next(void) const
  {
    if (fields_iterator_ == fields_.end()) {
        return false;
    }
    return true;
  }

  const Field *Document::get_next(void)
  {
    return static_cast<const Document&>(*this).get_next();
  }

  const Field *Document::get_next(void) const
  {
    //return (*fields_iterator_++)->get();
    SPF field = *fields_iterator_++;
    return field.get();
  }

  void Document::init_iter(void)
  {
    fields_iterator_ = fields_.begin();
  }

  void Document::init_iter(void) const
  {
    fields_iterator_ = fields_.begin();
  }

  const std::string Document::get_eid(void)
  {
    return static_cast<const Document&>(*this).get_eid();
  }

  const std::string Document::get_eid(void) const
  {
    return external_id_;
  }

  void Document::set_id(doc_id_t internal_id)
  {
    internal_id_ = internal_id;
  }

  const doc_id_t Document::get_id(void)
  {
    return static_cast<const Document&>(*this).get_id();
  }

  const doc_id_t Document::get_id(void) const
  {
    return internal_id_;
  }

  const std::string Document::get_by_name(std::string field_name)
  {
    FCIterator itr_end = fields_.end();
    for (FCIterator itr = fields_.begin(); itr != itr_end; ++itr) {
      const Field *f = itr->get();
      if (f->get_name() == field_name) {
        return f->get_value();
      }
    }
    // returning empty string when no field found
    return std::string("");
  }

}
