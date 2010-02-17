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

  /**
   * document passed to indexer
   */
  class Document {
  public:
    /**
     * constructor
     * @param id external ID in char *
     */
    Document(char *id);
    /**
     * constructor
     * @param id external ID in std::string
     */
    Document(std::string &id);
    /**
     * copy constructor
     */
    Document(const Document &doc);
    /**
     * assignment operator
     */
    Document &operator=(const Document &rhs);
    /**
     * destructor
     */
    ~Document(void);
    /**
     * add a field
     * @param field pointer to Field 
     */
    void add(const Field *field);
    /**
     * check if it has a next field
     */
    bool has_next(void);
    /**
     * check if it has a next field
     */
    bool has_next(void) const;
    /**
     * get a next pointer to Field
     */
    const Field *get_next(void);
    /**
     * get a next pointer to Field
     */
    const Field *get_next(void) const;
    /**
     * initialize an iterator
     */
    void init_iter(void);
    /**
     * initialize an iterator
     */
    void init_iter(void) const;
    /**
     * get the external ID
     */
    const std::string get_eid(void);
    /**
     * get the external ID
     */
    const std::string get_eid(void) const;
    /**
     * set a internal ID
     */
    void set_id(doc_id_t internal_id);
    /**
     * get the internal ID
     */
    const doc_id_t get_id(void);
    /**
     * get the internal ID
     */
    const doc_id_t get_id(void) const;
    /**
     * get the field value of the specified field name
     */
    const std::string get_by_name(std::string field_name);

  private:
    doc_id_t internal_id_;
    std::string external_id_; // set in construction
    FieldContainer fields_;
    mutable FCIterator fields_iterator_;
  };
}

#endif
