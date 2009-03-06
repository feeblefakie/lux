#ifndef LUX_STORAGE_VBC_H
#define LUX_STORAGE_VBC_H

#include <vector>
#include <deque>
#include <stdint.h>

// test
#include <boost/pool/pool_alloc.hpp>

namespace Lux {
  
  //typedef std::vector< uint32_t,
  //                     boost::pool_allocator<uint32_t> > ui32_vec_t;
  //typedef std::vector<uint32_t> ui32_vec_t;
  typedef std::deque<uint32_t> ui32_vec_t;

  static inline
  void vb_encode_num(int n, std::vector<uint8_t> &bytestream)
  {
    std::vector<uint8_t> bytes;
    bytes.reserve(8);
    while (true) {
      unsigned char rem = n % 128;
      bytes.push_back(rem);
      if (n < 128) break;
      n /= 128;
    }
    bytes[0] += 128;
    size_t size = bytes.size();
    for (int i = size - 1; i >= 0; --i) {
      bytestream.push_back(bytes[i]); 
    }
  }

  static inline
  std::vector<uint8_t> vb_encode_nodiff(std::vector<uint32_t> &numbers)
  {
    std::vector<uint8_t> bytestream;
    std::vector<uint32_t>::iterator itr = numbers.begin();
    std::vector<uint32_t>::iterator itr_end = numbers.end();
    for (; itr != itr_end; ++itr) {
      vb_encode_num(*itr, bytestream);
    }
    return bytestream;
  }

  static inline
  std::vector<uint8_t> vb_encode_docdiff(std::vector<uint32_t> &numbers)
  {
    std::vector<uint8_t> bytestream;
    std::vector<uint32_t>::iterator itr = numbers.begin();
    std::vector<uint32_t>::iterator itr_end = numbers.end();
    int num_postings = 0;

    for (; itr != itr_end; ++itr) {
      if (num_postings == 0) {
        vb_encode_num(*itr, bytestream);
        num_postings = *(++itr);
        vb_encode_num(num_postings, bytestream);
      } else {
        int prev_pos = 0;
        while (itr != itr_end) {
          vb_encode_num(*itr-prev_pos, bytestream);
          prev_pos = *itr;
          if (--num_postings > 0) {
            ++itr;
          } else {
            break;
          }
        }
      }
    }
    return bytestream;
  }

  // handle both doc and position differences
  static inline
  //std::vector<uint8_t> vb_encode(std::vector<uint32_t> &numbers)
  std::vector<uint8_t> vb_encode(ui32_vec_t &numbers)
  {
    std::vector<uint8_t> bytestream;
    //std::vector<uint32_t>::iterator itr = numbers.begin();
    ui32_vec_t::iterator itr = numbers.begin();
    //std::vector<uint32_t>::iterator itr_end = numbers.end();
    ui32_vec_t::iterator itr_end = numbers.end();
    int prev_doc_id = 0;
    int doc_id_gap;
    int num_postings = 0;

    int i = 0;
    for (; itr != itr_end; ++itr) {
      if (num_postings == 0) {
        ++i;
        doc_id_gap = *itr - prev_doc_id;
        prev_doc_id = *itr;
        vb_encode_num(doc_id_gap, bytestream);
        num_postings = *(++itr);
        vb_encode_num(num_postings, bytestream);
      } else {
        int prev_pos = 0;
        while (itr != itr_end) {
          vb_encode_num(*itr-prev_pos, bytestream);
          prev_pos = *itr;
          if (--num_postings > 0) {
            ++itr;
          } else {
            break;
          }
        }
      }
    }
    return bytestream;
  }

  static inline
  std::vector<uint32_t> vb_decode_nodiff(uint8_t *bytestream, size_t size, bool skip_pos=false)
  {
    int n = 0;
    std::vector<uint32_t> array;
    array.reserve(5*size);

    for (int i = 0; i < size; ++i) {
      n = n * 128 + bytestream[i];
      if (bytestream[i] >= 128) {
        array.push_back(n-128);
        if (skip_pos) {
          skip(bytestream, i , size, next(bytestream, ++i, size));
        }
        n = 0;
      }
    }
    return array;
  }

  static inline
  std::vector<uint32_t> vb_decode_docdiff(uint8_t *bytestream, size_t size, bool skip_pos=false)
  {
    int n = 0;
    std::vector<uint32_t> array;
    array.reserve(5*size);

    for (int i = 0; i < size; ++i) {
      n = n * 128 + bytestream[i];
      if (bytestream[i] >= 128) {
        array.push_back(n-128);
        n = 0;
        if (skip_pos) {
          skip(bytestream, i , size, next(bytestream, ++i, size));
        } else {
          uint32_t num_postings = next(bytestream, ++i, size);
          array.push_back(num_postings);
          uint32_t prev_pos = 0;
          for (int j = 0; j < num_postings && i < size; ++i) {
            n = n * 128 + bytestream[i];
            if (bytestream[i] >= 128) {
              array.push_back(n - 128 + prev_pos);
              prev_pos = n - 128 + prev_pos;
              n = 0;
              ++j;
            }
          }
        }
        --i;
      }
    }
    return array;
  }

  // handle both doc and position differences
  static inline
  //std::vector<uint32_t> vb_decode(uint8_t *bytestream, size_t size, bool skip_pos=false)
  ui32_vec_t vb_decode(uint8_t *bytestream, size_t size, bool skip_pos=false)
  {
    int n = 0;
    //std::vector<uint32_t> array;
    ui32_vec_t array;
    //array.reserve(5*size);
    uint32_t prev_doc_id = 0;

    for (int i = 0; i < size; ++i) {
      if (bytestream[i] == 128 && n == 0) {
        prev_doc_id = 0;
        n = 0;
        continue;
      }
      n = n * 128 + bytestream[i];
      if (bytestream[i] >= 128) {
        array.push_back(n - 128 + prev_doc_id);
        prev_doc_id = n - 128 + prev_doc_id;
        n = 0;
        if (skip_pos) {
          skip(bytestream, i , size, next(bytestream, ++i, size));
        } else {
          uint32_t num_postings = next(bytestream, ++i, size);
          array.push_back(num_postings);
          uint32_t prev_pos = 0;
          for (int j = 0; j < num_postings && i < size; ++i) {
            n = n * 128 + bytestream[i];
            if (bytestream[i] >= 128) {
              array.push_back(n - 128 + prev_pos);
              prev_pos = n - 128 + prev_pos;
              n = 0;
              ++j;
            }
          }
        }
        --i;
      }
    }
    return array;
  }
}

#endif
