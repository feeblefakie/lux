#ifndef LUX_DOCUMENT_H
#define LUX_DOCUMENT_H

#include "lux/lux.h"
#include "field.h"
#include <vector>
#include <string>

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
    Document(char *id);
    Document(std::string &id);
    Document(const Document &doc);
    Document &operator=(const Document &rhs);
    ~Document(void);
    void add(const Field *field);
    bool has_next(void);
    bool has_next(void) const;
    const Field *get_next(void);
    const Field *get_next(void) const;
    void init_iter(void);
    void init_iter(void) const;
    const std::string get_eid(void);
    const std::string get_eid(void) const;
    void set_id(doc_id_t internal_id);
    const doc_id_t get_id(void);
    const doc_id_t get_id(void) const;
    const std::string get_by_name(std::string field_name);

  private:
    doc_id_t internal_id_;
    std::string external_id_; // set in construction
    FieldContainer fields_;
    mutable FCIterator fields_iterator_;
  };
}

#endif
