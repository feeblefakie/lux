#ifndef LUX_DOCUMENT_H
#define LUX_DOCUMENT_H

#include "lux/lux.h"
#include "Field.h"
#include <vector>
#include <iostream>

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

#define ERR_DOC_ID "__ERRDOC__"

namespace Lux {

#ifdef HAVE_TR1_MEMORY
  typedef std::tr1::shared_ptr<const Lux::Field> SPF;
#elif HAVE_BOOST_SHARED_PTR_HPP
  typedef boost::shared_ptr<const Lux::Field> SPF;
#endif
  typedef std::vector<SPF> FieldContainer;
  typedef FieldContainer::const_iterator FCIterator;

  class Document {
      
  public:
    Document(char *id)
    : external_id_(id)
    {}

    Document(std::string &id)
    : external_id_(id)
    {}

    Document(const Document &doc)
    : external_id_(doc.external_id_), fields_(doc.fields_)
    {
      fields_iterator_ = fields_.begin();
    }

    Document &operator=(const Document &rhs)
    {
      external_id_ = rhs.external_id_;
      fields_ = rhs.fields_;
      fields_iterator_ = fields_.begin();
      return *this;
    }

    ~Document()
    {}

    void add(const Field *field)
    {
      fields_.push_back(SPF(field));
      fields_iterator_ = fields_.begin();
    }

    bool has_next(void)
    {
      return static_cast<const Document&>(*this).has_next();
    }

    bool has_next(void) const
    {
      if (fields_iterator_ == fields_.end()) {
          return false;
      }
      return true;
    }

    const Field *get_next(void)
    {
      return static_cast<const Document&>(*this).get_next();
    }

    const Field *get_next(void) const
    {
      //return (*fields_iterator_++)->get();
      SPF field = *fields_iterator_++;
      return field.get();
    }

    void init_iter(void)
    {
      fields_iterator_ = fields_.begin();
    }

    void init_iter(void) const
    {
      fields_iterator_ = fields_.begin();
    }

    const std::string get_eid(void)
    {
      return static_cast<const Document&>(*this).get_eid();
    }

    const std::string get_eid(void) const
    {
      return external_id_;
    }

    void set_id(doc_id_t internal_id)
    {
      internal_id_ = internal_id;
    }

    const doc_id_t get_id(void)
    {
      return static_cast<const Document&>(*this).get_id();
    }

    const doc_id_t get_id(void) const
    {
      return internal_id_;
    }

    const std::string get_by_name(std::string field_name)
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

  private:
    doc_id_t internal_id_;
    std::string external_id_; // set in construction
    FieldContainer fields_;
    mutable FCIterator fields_iterator_;
  };
}

#endif
