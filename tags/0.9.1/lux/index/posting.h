#ifndef LUX_INDEX_POSTING_H
#define LUX_INDEX_POSTING_H

#include <vector>
#include <deque>
#include <iostream>
#include <stdint.h>

// test
#include <boost/pool/pool_alloc.hpp>

namespace Lux {

  //typedef std::vector< uint32_t,
  //                     boost::pool_allocator<uint32_t> > pos_list_t;
  //typedef std::vector<uint32_t> pos_list_t;
  typedef std::deque<uint32_t> pos_list_t;
  typedef pos_list_t::iterator pos_list_itr;

  struct Posting {
    Posting(void)
    {}
    Posting(pos_list_t _list, bool _skipped)
    : list(_list), skipped(_skipped)
    {}

    pos_list_t list;
    bool skipped;
  };

  struct PostingInf {
    PostingInf(uint32_t size_, uint32_t idx_)
    : size(size_), idx(idx_)
    {}
    uint32_t size;
    uint32_t idx;
  };
  typedef std::vector<PostingInf> PostingInfs;
  typedef PostingInfs::iterator PostingInfsItr;

  struct sort_by_size : public std::binary_function<PostingInf, PostingInf, bool> {
    bool operator()(const PostingInf &t1, const PostingInf &t2) {
      return t1.size < t2.size;
    }
  };

  /**
   * 
   *
   */

  // next number for uint8_t *
  static inline
  int32_t next(uint8_t *b, int &i, int size)
  {
    uint32_t n = 0;
    for (; i < size; ++i) {
      n = 128 * n + b[i];
      if (b[i] >= 128) {
        n -= 128;
        ++i;
        return n;
      }
    } 
    return -1;
  }

  static inline
  int32_t next(uint8_t *b, int &i, int size, uint32_t &prev)
  {
    uint32_t dp = next(b, i, size);
    if (dp > 0) {
      dp += prev;
    } else if (dp == 0) {
      do {
        dp = next(b, i, size);
      } while (dp == 0);
    }
    prev = dp;
    return dp;
  }

  static inline
  void skip(uint8_t *b, int &i, int size, int skip)
  {
    for (; i < size; ++i) {
      if (b[i] >= 128) {
        --skip;
        if (skip == 0) {
          ++i;
          return;
        }
      }
    } 
  }

  // next doc for pos_list_t
  static inline
  int32_t next(pos_list_itr *itr, pos_list_itr itr_end)
  {
    if (*itr != itr_end) {
      *itr += *(++*itr) + 1;
      if (*itr != itr_end) {
        return **itr;
      }
    }
    return -1;
  }

  // next position for pos_list_t
  static inline
  int32_t next(pos_list_itr *itr, int *n) {
    --*n;
    if (*n >= 0) {
      return *(++*itr);
    }
    return -1;
  }

  // intersection between uint8_t* arrays for postions
  static inline pos_list_t
  _intersect_nodiff(uint8_t *bp, int &ip, int sp,
             uint8_t *bn, int &in, int sn, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = next(bp, ip, sp) - 1;
    int nn = next(bn, in, sn) - 1;
    int pp = next(bp, ip, sp);
    int pn = next(bn, in, sn);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        if (np > 0) { pp = next(bp, ip, sp); }
        if (nn > 0) { pn = next(bn, in, sn); }
        --nn;
        --np;
      } else if (pp + len > pn) {
        if (nn > 0) { pn = next(bn, in, sn); }
        --nn;
      } else {
        if (np > 0) { pp = next(bp, ip, sp); }
        --np;
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        if (nn > 0) { skip(bn, in, sn, nn); }
        break;
      }
      if (nn < 0 && pp + len > pn) {
        if (np > 0) { skip(bp, ip, sp, np); }
        break;  
      }
    }
    return pl;
  }

  // intersection between uint8_t* arrays for postions
  static inline pos_list_t
  _intersect(uint8_t *bp, int &ip, int sp,
              uint8_t *bn, int &in, int sn, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = next(bp, ip, sp) - 1;
    int nn = next(bn, in, sn) - 1;
    int pp = next(bp, ip, sp);
    int pn = next(bn, in, sn);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        if (np > 0) { pp += next(bp, ip, sp); }
        if (nn > 0) { pn += next(bn, in, sn); }
        --nn;
        --np;
      } else if (pp + len > pn) {
        if (nn > 0) { pn += next(bn, in, sn); }
        --nn;
      } else {
        if (np > 0) { pp += next(bp, ip, sp); }
        --np;
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        if (nn > 0) { skip(bn, in, sn, nn); }
        break;
      }
      if (nn < 0 && pp + len > pn) {
        if (np > 0) { skip(bp, ip, sp, np); }
        break;  
      }
    }
    return pl;
  }

  // intersection between uint8_t* arrays for docid
  static inline 
  pos_list_t intersect_nodiff(uint8_t *bp, uint32_t sp,
                              uint8_t *bn, uint32_t sn, int len)
  {
    pos_list_t pl;
    if (sp == 0 || sn == 0) { return pl; }
    //pl.reserve(sp > sn ? sp : sn);

    int ip = 0, in = 0;
    int dp = next(bp, ip, sp);
    int dn = next(bn, in, sn);

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect_nodiff(bp, ip , sp, bn, in, sn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(bp, ip, sp);
        dn = next(bn, in, sn);
      } else if (dp > dn) {
        skip(bn, in, sn, next(bn, in, sn));
        dn = next(bn, in, sn);
      } else {
        skip(bp, ip, sp, next(bp, ip, sp));
        dp = next(bp, ip, sp);
      }

      if ((ip == sp && in == sn) ||
          (ip == sp && dn > dp) || 
          (in == sn && dp > dn)) {
          break;
      }
    }
    return pl;
  }

  // intersection between uint8_t* arrays for docid
  static inline 
  pos_list_t intersect(uint8_t *bp, uint32_t sp,
                        uint8_t *bn, uint32_t sn, int len)
  {
    pos_list_t pl;
    if (sp == 0 || sn == 0) { return pl; }
    //pl.reserve(sp > sn ? sp : sn);

    int ip = 0, in = 0;
    uint32_t prev_dp = 0, prev_dn = 0;
    int dp = next(bp, ip, sp, prev_dp);
    int dn = next(bn, in, sn, prev_dn);

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect(bp, ip, sp, bn, in, sn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(bp, ip, sp, prev_dp);
        dn = next(bn, in, sn, prev_dn);
      } else if (dp > dn) {
        skip(bn, in, sn, next(bn, in, sn));
        dn = next(bn, in, sn, prev_dn);
      } else {
        skip(bp, ip, sp, next(bp, ip, sp));
        dp = next(bp, ip, sp, prev_dp);
      }

      if ((ip == sp && in == sn) ||
          (ip == sp && dn > dp) || 
          (in == sn && dp > dn)) {
          break;
      }
    }
    return pl;
  }


  // intersection between uint8_t* array and pos_list_t for positions
  static inline pos_list_t
  _intersect_nodiff(uint8_t *bp, int &ip, int sp, pos_list_itr itr, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = next(bp, ip, sp) - 1;
    int nn = *(++itr) - 1;
    int pp = next(bp, ip, sp);
    int pn = *(++itr);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        if (np > 0) { pp = next(bp, ip, sp); }
        pn = next(&itr, &nn);
        --np;
      } else if (pp + len > pn) {
        pn = next(&itr, &nn);
      } else {
        if (np > 0) { pp = next(bp, ip, sp); }
        --np;
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        break;
      }
      if (nn < 0 && pp + len > pn) {
        if (np > 0) { skip(bp, ip, sp, np); }
        break;  
      }
    }
    return pl;
  }

  // intersection between uint8_t* array and pos_list_t for positions
  static inline pos_list_t
  _intersect(uint8_t *bp, int &ip, int sp, pos_list_itr itr, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = next(bp, ip, sp) - 1;
    int nn = *(++itr) - 1;
    int pp = next(bp, ip, sp);
    int pn = *(++itr);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        if (np > 0) { pp += next(bp, ip, sp); }
        pn = next(&itr, &nn);
        --np;
      } else if (pp + len > pn) {
        pn = next(&itr, &nn);
      } else {
        if (np > 0) { pp += next(bp, ip, sp); }
        --np;
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        break;
      }
      if (nn < 0 && pp + len > pn) {
        if (np > 0) { skip(bp, ip, sp, np); }
        break;  
      }
    }
    return pl;
  }

  // intersection between uint8_t* array and pos_list_t for docid
  static inline 
  pos_list_t intersect_nodiff(uint8_t *bp, uint32_t sp, pos_list_t &pn, int len)
  {
    pos_list_t pl;
    if (sp == 0 || pn.size() == 0) { return pl; }
    //pl.reserve(sp > pn.size() ? sp : pn.size());

    pos_list_itr itrn = pn.begin();
    pos_list_itr itrn_end = pn.end();

    int ip = 0;
    int dp = next(bp, ip, sp);
    int dn = *itrn;

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect_nodiff(bp, ip , sp, itrn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(bp, ip, sp);
        dn = next(&itrn, itrn_end);
      } else if (dp > dn) {
        dn = next(&itrn, itrn_end);
      } else {
        skip(bp, ip, sp, next(bp, ip, sp));
        dp = next(bp, ip, sp);
      }

      if ((ip == sp && itrn == itrn_end) ||
          (ip == sp && dn > dp) || 
          (itrn == itrn_end && dp > dn)) {
          break;
      }
    }
    return pl;
  }

  // intersection between uint8_t* array and pos_list_t for docid
  static inline 
  pos_list_t intersect(uint8_t *bp, uint32_t sp, pos_list_t &pn, int len)
  {
    pos_list_t pl;
    if (sp == 0 || pn.size() == 0) { return pl; }
    //pl.reserve(sp > pn.size() ? sp : pn.size());

    pos_list_itr itrn = pn.begin();
    pos_list_itr itrn_end = pn.end();

    int ip = 0;
    uint32_t prev_dp = 0;
    int dp = next(bp, ip, sp, prev_dp);
    int dn = *itrn;

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect(bp, ip , sp, itrn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(bp, ip, sp, prev_dp);
        dn = next(&itrn, itrn_end);
      } else if (dp > dn) {
        dn = next(&itrn, itrn_end);
      } else {
        skip(bp, ip, sp, next(bp, ip, sp));
        dp = next(bp, ip, sp, prev_dp);
      }

      if ((ip == sp && itrn == itrn_end) ||
          (ip == sp && dn > dp) || 
          (itrn == itrn_end && dp > dn)) {
          break;
      }
    }
    return pl;
  }

  // intersection between pos_list_t and uint8_t* array for positions
  static inline pos_list_t
  _intersect_nodiff(pos_list_itr itr, uint8_t *bn, int &in, int sn, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = *(++itr) - 1;
    int nn = next(bn, in, sn) - 1;
    int pp = *(++itr);
    int pn = next(bn, in, sn);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        pp = next(&itr, &np);
        if (nn > 0) { pn = next(bn, in, sn); }
        --nn;
      } else if (pp + len > pn) {
        if (nn > 0) { pn = next(bn, in, sn); }
        --nn;
      } else {
        pp = next(&itr, &np);
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        if (nn > 0) { skip(bn, in, sn, nn); }
        break;
      }
      if (nn < 0 && pp + len > pn) {
        break;  
      }
    }
    return pl;
  }

  // intersection between pos_list_t and uint8_t* array for positions
  static inline pos_list_t
  _intersect(pos_list_itr itr, uint8_t *bn, int &in, int sn, int len)
  {
    pos_list_t pl;
    //pl.reserve(128); // for eliminating unnecessary copies

    int np = *(++itr) - 1;
    int nn = next(bn, in, sn) - 1;
    int pp = *(++itr);
    int pn = next(bn, in, sn);

    while (1) {
      if (pp + len == pn) {
        pl.push_back(pp);
        pp = next(&itr, &np);
        if (nn > 0) { pn += next(bn, in, sn); }
        --nn;
      } else if (pp + len > pn) {
        if (nn > 0) { pn += next(bn, in, sn); }
        --nn;
      } else {
        pp = next(&itr, &np);
      }

      if (np < 0 && nn < 0) break;
      if (np < 0 && pn > pp + len) {
        if (nn > 0) { skip(bn, in, sn, nn); }
        break;
      }
      if (nn < 0 && pp + len > pn) {
        break;  
      }
    }
    return pl;
  }

  // intersection between pos_list_t and uint8_t* array for docid
  static inline 
  pos_list_t intersect_nodiff(pos_list_t &pp, uint8_t *bn, uint32_t sn, int len)
  {
    pos_list_t pl;
    if (pp.size() == 0 || sn == 0) { return pl; }
    //pl.reserve(pp.size() > sn ? pp.size() : sn);

    pos_list_itr itrp = pp.begin();
    pos_list_itr itrp_end = pp.end();

    int in = 0;
    int dp = *itrp;
    int dn = next(bn, in, sn);

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect_nodiff(itrp, bn, in , sn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(&itrp, itrp_end);
        dn = next(bn, in, sn);
      } else if (dp > dn) {
        skip(bn, in, sn, next(bn, in, sn));
        dn = next(bn, in, sn);
      } else {
        dp = next(&itrp, itrp_end);
      }

      if ((itrp == itrp_end && in == sn) ||
          (itrp == itrp_end && dn > dp) || 
          (in == sn && dp > dn)) {
          break;
      }
    }
    return pl;
  }

  // intersection between pos_list_t and uint8_t* array for docid
  static inline 
  pos_list_t intersect(pos_list_t &pp, uint8_t *bn, uint32_t sn, int len)
  {
    pos_list_t pl;
    if (pp.size() == 0 || sn == 0) { return pl; }
    //pl.reserve(pp.size() > sn ? pp.size() : sn);

    pos_list_itr itrp = pp.begin();
    pos_list_itr itrp_end = pp.end();

    int in = 0;
    int dp = *itrp;
    uint32_t prev_dn = 0;
    int dn = next(bn, in, sn, prev_dn);

    while (1) {
      if (dp == dn) {
        pos_list_t tp = _intersect(itrp, bn, in , sn, len);
        if (!tp.empty()) {
          pl.push_back(dp);
          pl.push_back(tp.size());
          pos_list_itr itr_end = tp.end();
          for (pos_list_itr itr = tp.begin();
               itr != itr_end; ++itr) {
            pl.push_back(*itr);
          }
        }
        dp = next(&itrp, itrp_end);
        dn = next(bn, in, sn, prev_dn);
      } else if (dp > dn) {
        skip(bn, in, sn, next(bn, in, sn));
        dn = next(bn, in, sn, prev_dn);
      } else {
        dp = next(&itrp, itrp_end);
      }

      if ((itrp == itrp_end && in == sn) ||
          (itrp == itrp_end && dn > dp) || 
          (in == sn && dp > dn)) {
          break;
      }
    }
    return pl;
  }

}

#endif
