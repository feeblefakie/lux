#ifndef LUX_BOOLEANMERGER_H
#define LUX_BOOLEANMERGER_H

#include <iostream>
#include "SearchIndex.h"

namespace Lux {

  class BooleanMerger {

  public:
    static IndexResultSet AND(IndexResultSet &rs1, IndexResultSet &rs2)
    {
      IRSIterator itr1 = rs1.begin();
      IRSIterator itr2 = rs2.begin();
      IRSIterator itr1_end = rs1.end();
      IRSIterator itr2_end = rs2.end();
      IRSIterator itr1_cur, itr2_cur;
      IndexResultSet rs;

      if (itr1 == itr1_end || itr2 == itr2_end) {
        return rs;
      }

      rs.reserve(rs1.size() > rs2.size() ?  rs1.size() : rs2.size());
      while (1) {
        if (itr1 != itr1_end) { itr1_cur = itr1; }
        if (itr2 != itr2_end) { itr2_cur = itr2; }

        if (itr1_cur->doc_id == itr2_cur->doc_id) {
          unsigned int score = (itr1_cur->score + itr2_cur->score) / 2;
          rs.push_back(IndexResult(itr1_cur->doc_id, score));
          if (itr1 != itr1_end) { ++itr1; }
          if (itr2 != itr2_end) { ++itr2; }
        } else if (itr1_cur->doc_id > itr2_cur->doc_id) {
          if (itr2 != itr2_end) { ++itr2; }
        } else {
          if (itr1 != itr1_end) { ++itr1; }
        }

        if ((itr1 == itr1_end && itr2 == itr2_end) ||
            (itr1 == itr1_end && itr2->doc_id > itr1_cur->doc_id) || 
            (itr2 == itr2_end && itr1->doc_id > itr2_cur->doc_id)) {
            break;
        }
      }
      return rs;
    }
    
    static IndexResultSet OR(IndexResultSet &rs1, IndexResultSet &rs2)
    {
      IRSIterator itr1 = rs1.begin();
      IRSIterator itr2 = rs2.begin();
      IRSIterator itr1_end = rs1.end();
      IRSIterator itr2_end = rs2.end();
      IRSIterator itr1_cur, itr2_cur;
      IndexResultSet rs;

      if (itr1 == itr1_end && itr2 == itr2_end) {
        return rs;
      } else if (itr1 == itr1_end) {
        return rs2;
      } else if (itr2 == itr2_end) {
        return rs1;
      }

      rs.reserve(rs1.size() > rs2.size() ?  rs1.size() : rs2.size());
      while (1) {
        if (itr1 != itr1_end) { itr1_cur = itr1; }
        if (itr2 != itr2_end) { itr2_cur = itr2; }

        if (itr1_cur->doc_id == itr2_cur->doc_id) {
          // choose more relevanced one
          if (itr1_cur->score > itr2_cur->score) {
            rs.push_back(*itr1_cur);
          } else {
            rs.push_back(*itr2_cur);
          }
          if (itr1 != itr1_end) { ++itr1; }
          if (itr2 != itr2_end) { ++itr2; }
        } else if (itr1_cur->doc_id > itr2_cur->doc_id) {
          rs.push_back(*itr2_cur);
          if (itr2 != itr2_end) { ++itr2; }
        } else {
          rs.push_back(*itr1_cur);
          if (itr1 != itr1_end) { ++itr1; }
        }

        if ((itr1 == itr1_end) && (itr2 == itr2_end)) {
            break;
        }
        if (itr1 == itr1_end && itr2->doc_id > itr1_cur->doc_id) {
          for (; itr2 != itr2_end; ++itr2) {
            rs.push_back(*itr2);
          }
          break;
        }
        if (itr2 == itr2_end && itr1->doc_id > itr2_cur->doc_id) {
          for (; itr1 != itr1_end; ++itr1 ) {
            rs.push_back(*itr1);
          }
          break;
        }
      }
      return rs;
    }

    static IndexResultSet NOT(IndexResultSet &rs1, IndexResultSet &rs2)
    {
      IRSIterator itr1 = rs1.begin();
      IRSIterator itr2 = rs2.begin();
      IRSIterator itr1_end = rs1.end();
      IRSIterator itr2_end = rs2.end();
      IRSIterator itr1_cur, itr2_cur;
      IndexResultSet rs;

      if (itr1 == itr1_end) {
        return rs;
      }
      if (itr2 == itr2_end) {
        return rs1;
      }

      rs.reserve(rs1.size() > rs2.size() ?  rs1.size() : rs2.size());
      while (1) {
        if (itr1 != itr1_end) { itr1_cur = itr1; }
        if (itr2 != itr2_end) { itr2_cur = itr2; }

        if (itr1_cur->doc_id == itr2_cur->doc_id) {
          if (itr1 != itr1_end) { ++itr1; }
          if (itr2 != itr2_end) { ++itr2; }
        } else if (itr1_cur->doc_id > itr2_cur->doc_id) {
          if (itr2 != itr2_end) { ++itr2; }
        } else {
          rs.push_back(*itr1_cur);
          if (itr1 != itr1_end) { ++itr1; }
        }

        if (itr1 == itr1_end) { break; }

        if (itr2 == itr2_end && itr1->doc_id > itr2_cur->doc_id) {
          for (; itr1 != itr1_end; ++itr1 ) {
            rs.push_back(*itr1);
          }
          break;
        }
      }
      return rs;
    }
 
  };
}

#endif
