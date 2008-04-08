#ifndef LUX_POSTING_H
#define LUX_POSTING_H

#include <vector>
#include <iostream>

/* write integer(_num) to the area the pointer(_buf) points */
#define INTARRAY_WRITE(_buf, _num, _len) \
  do { \
    int __num = (_num); \
    div_t _d; \
    if (__num == 0) { \
      ((signed char *) (_buf))[0] = 0; \
      (_len) = 1; \
    } else { \
      (_len) = 0; \
      while (__num > 0) { \
        _d = div(__num, 128); \
        __num = _d.quot; \
        if(__num > 0) { \
          ((signed char *) (_buf))[(_len)] = -_d.rem - 1; \
        } else { \
          ((signed char *) (_buf))[(_len)] = _d.rem; \
        } \
        (_len)++; \
      } \
    } \
  } while (false)

/* read integer(_num) from the area the pointer(_buf) points */
#define INTARRAY_READ(_buf, _num, _step) \
  do { \
    int _i, _base; \
    (_num) = 0; \
    _base = 1; \
    for (_i = 0; true; ++_i) { \
      if (((signed char *) (_buf))[_i] >= 0) { \
        (_num) += ((signed char *) (_buf))[_i] * _base; \
        break; \
      } \
      (_num) += _base * (((signed char *) (_buf))[_i] + 1) * -1; \
      _base <<= 7; \
    } \
    _step = _i + 1; \
  } while (false)

namespace Lux {

  // must be compatible with OccList in SearchIndex.h
  typedef std::vector<unsigned int> PostingList;
  typedef PostingList::iterator PLIterator;
  typedef std::vector<int> Offsets;
  typedef Offsets::iterator OIterator;

  typedef std::vector<PostingList> PLArray;
  typedef PLArray::iterator PLAIterator;

  struct PLA_elem_inf {
    PLA_elem_inf(unsigned int plist_size_, unsigned int pla_idx_)
    : plist_size(plist_size_), pla_idx(pla_idx_)
    {}
    unsigned int plist_size;
    unsigned int pla_idx;
  };
  typedef std::vector<PLA_elem_inf> PLA_inf;
  typedef PLA_inf::iterator PLAIIterator;

  struct sort_by_size : public std::binary_function<PLA_elem_inf, PLA_elem_inf, bool> {
    bool operator()(const PLA_elem_inf &t1, const PLA_elem_inf &t2) {
      return t1.plist_size < t2.plist_size;
    }
  };

  /* get INTARRAY from PostingList */
  inline
  char *get_intarray_from(PostingList &plist, int *intarray_len)
  {
    if (plist.empty()) { return NULL; }

    char *p = new char[plist.size() * sizeof(int)];
    char *p0 = &p[0];
    PLIterator itr_end = plist.end();
    *intarray_len = 0;

    for (PLIterator itr = plist.begin(); itr != itr_end; ++itr) {
      int len;
      INTARRAY_WRITE(p, *itr, len);
      p += len; 
      *intarray_len += len; 
    }
    return p0;
  }

  /* get PostingList from INTARRAY */
  inline
  PostingList get_plist_from(char *p, unsigned int len)
  { 
    PostingList plist;
    if (p == NULL) {
      return plist;
    }

    while (len > 0) {
      int num, step;
      INTARRAY_READ(p, num, step);
      p += step;
      len -= step;
      plist.push_back(num);
    }
    return plist;
  }

  static inline
  void next_doc(PLIterator *itr, PLIterator itr_end, int *doc)
  {
    if (*itr != itr_end) {
      *itr += *(++*itr) + 1;
      if (*itr != itr_end) {
        *doc = **itr;
      }
    }
  }

  static inline
  void next_offset(PLIterator *itr, int *counter, int *val) {
    --*counter;
    if (*counter >= 0) {
      *val = *(++*itr);
    }
  }

  static inline
  Offsets get_contiguous_offsets(PLIterator itr_p, PLIterator itr_n, int term_len)
  {
    Offsets o;
    o.reserve(32); // for eliminating unnecessary copies

    int cnt_p = *(++itr_p) - 1;
    int cnt_n = *(++itr_n) - 1;
    int off_p = *(++itr_p);
    int off_n = *(++itr_n);

    while (1) {
      if (off_p + term_len == off_n) {
        o.push_back(off_p);
        next_offset(&itr_p, &cnt_p, &off_p);
        next_offset(&itr_n, &cnt_n, &off_n);
      } else if (off_p + term_len > off_n) {
        next_offset(&itr_n, &cnt_n, &off_n);
      } else {
        next_offset(&itr_p, &cnt_p, &off_p);
      }


      if ((cnt_p < 0 && cnt_n < 0) || 
          (cnt_p < 0 && off_n > off_p + term_len) ||
          (cnt_n < 0 && off_p + term_len > off_n)) {
        break;  
      }
    }

    return o;
  }

  inline 
  PostingList merge_plist(PostingList &plist_p, PostingList &plist_n, int term_len)
  {
    PLIterator itr_p = plist_p.begin();
    PLIterator itr_n = plist_n.begin();
    PLIterator itr_p_end = plist_p.end();
    PLIterator itr_n_end = plist_n.end();

    PostingList plist;
    plist.reserve(plist_p.size() > plist_n.size() ? 
                  plist_p.size() : plist_n.size());
    int doc_p = *itr_p;
    int doc_n = *itr_n;
    while (1) {
      if (doc_p == doc_n) {
        Offsets o = get_contiguous_offsets(itr_p, itr_n, term_len);
        if (!o.empty()) {
          plist.push_back(doc_p);
          plist.push_back(o.size());
          OIterator itr_end = o.end();
          for (OIterator itr = o.begin(); itr != itr_end; ++itr) {
            plist.push_back(*itr);
          }
        }
        next_doc(&itr_p, itr_p_end, &doc_p);
        next_doc(&itr_n, itr_n_end, &doc_n);
      } else if (doc_p > doc_n) {
        next_doc(&itr_n, itr_n_end, &doc_n);
      } else {
        next_doc(&itr_p, itr_p_end, &doc_p);
      }

      if ((itr_p == itr_p_end && itr_n == itr_n_end) ||
          (itr_p == itr_p_end && doc_n > doc_p) || 
          (itr_n == itr_n_end && doc_p > doc_n)) {
          break;
      }
    }
    return plist;
  }

}

#endif
