#ifndef LUX_DIST_DISPATCHER_H
#define LUX_DIST_DISPATCHER_H

#include "handler.h"
#include "cluster_searcher.h"

namespace Lux {
namespace Dist {

  class Dispatcher : public Handler {
  public:
    Dispatcher(void *p);
    virtual ~Dispatcher(void);

  private:
#ifdef HAVE_TR1_MEMORY
    std::tr1::shared_ptr<ClusterSearcher> s_;
#elif HAVE_BOOST_SHARED_PTR_HPP
    boost::shared_ptr<ClusterSearcher> s_;
#endif

    virtual bool exec(request_header_t &header,
                      char *body, uint32_t body_len);
    bool exec_dispatch(char *body, uint32_t len);
    bool serialize_rs(TmpResultSet &rs, char **data, uint32_t *size);
  };

}
}

#endif
