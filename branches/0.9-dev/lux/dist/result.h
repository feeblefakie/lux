#ifndef LUX_DIST_RESULT_H
#define LUX_DIST_RESULT_H

#include "lux/lux.h"
#include "lux/StorageInterface/Paging.h"
#include <vector>

namespace Lux {
namespace Dist {

  /**
   * MergeResultSet
   */
  struct MergeResult {
    // [TODO] need cluster_id ?
    uint32_t server_id;
    doc_id_t doc_id;
    score_t score;
    char *attr;
  };
  typedef std::vector<MergeResult> MergeResultSet;

  static inline
  void clear_mrs(MergeResultSet &mrs)
  {
    MergeResultSet::iterator itr_end = mrs.end();
    for (MergeResultSet::iterator itr = mrs.begin();
         itr != itr_end; ++itr) {
      delete [] itr->attr;
    }
  }

  /**
   * DocResultSet
   */
  struct DocResult {
    DocResult(doc_id_t _id, std::string &_data)
    : id(_id), data(_data)
    {}

    doc_id_t id;
    std::string data;
  };
  typedef std::vector<DocResult> DocResultSet;

  /**
   * ClusterResultSet
   */
  struct ClusterResult {
    ClusterResult(std::string &_doc, score_t _score)
    : doc(_doc), score(_score)
    {}

    std::string doc; // serialized by DocumentSerializer
    score_t score; // set 0 when sorted by an attribute
  };
  typedef std::vector<ClusterResult> ClusterResultSet;

  /**
   * TmpResultSet
   */
  struct TmpResultSet {
    TmpResultSet(void)
    {}

    TmpResultSet(Paging &_paging,
                 ClusterResultSet &_crs, uint32_t _total_doc_size)
    : paging(_paging), crs(_crs), total_doc_size(_total_doc_size)
    {}

    Paging paging;
    ClusterResultSet crs;
    uint32_t total_doc_size;
  };

}
}

#endif
