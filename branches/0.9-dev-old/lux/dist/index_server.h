#ifndef LUX_DIST_INDEXSERVER_H
#define LUX_DIST_INDEXSERVER_H

#include "handler.h"
#include "result.h"
#include "lux/search.h"
#include "lux/index.h"

#ifdef HAVE_TR1_MEMORY
#include <tr1/memory>
#elif HAVE_BOOST_SHARED_PTR_HPP
#include <boost/shared_ptr.hpp>
#endif

namespace Lux {
namespace Dist {

  class IndexServer : public Handler {
  public:
    IndexServer(void *p);
    virtual ~IndexServer(void);

  private:
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<Lux::Searcher> s_;
    std::tr1::shared_ptr<Lux::Indexer> i_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<Lux::Searcher> s_;
    boost::shared_ptr<Lux::Indexer> i_;
#endif

    virtual bool exec(request_header_t &header,
                      char *body, uint32_t body_len);
    bool exec_find(char *body, uint32_t len);
    bool exec_getdoc(char *body, uint32_t len);
    bool exec_update(char *body, uint32_t len);
    bool serialize_doc_rs(DocResultSet &drs,
                          char **data, uint32_t size);
    bool serialize_idx_rs(IndexResultSet &irs,Condition &cond,
                          char **data, uint32_t *size);
  };

}
}

#endif
