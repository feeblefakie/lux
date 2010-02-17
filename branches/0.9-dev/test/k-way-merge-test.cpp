#include <iostream>
#include <stdlib.h>
#include <limits>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <time.h>
#include <sys/time.h>

// merge k sorted lists
// each list's length is different like 1:2:4:8:16:...:2^(k-1)

typedef std::vector<int> ARRAY;
typedef ARRAY::iterator AITR;
typedef std::vector< ARRAY > ARRAYS;

int int_comp(const void *_a, const void *_b);
void initialize(ARRAYS &arrs);

ARRAY linear_search_merge(ARRAYS &arrs, int from, int to);
ARRAY heap_merge(ARRAYS &arrs, int from, int to);
ARRAY set_merge(ARRAYS &arrs, int from, int to);

struct VAL {
  VAL(int val_, int off_)
  : val(val_), off(off_)
  {}
  int val;
  int off;
};

class greater_VAL : public std::binary_function<VAL, VAL, bool>
{
  public:
  result_type operator() (first_argument_type &a, second_argument_type &b)
  {
    return (result_type)((a.val > b.val) ? 1 : 0);
  }
};

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

// default values
int k = 20;
int size = 1000000;

int main(int argc, char *argv[])
{
  ARRAYS arrs;
  initialize(arrs);

  ARRAY (*merge)(ARRAYS &arrs, int from, int to);
  merge = linear_search_merge;
  //merge = heap_merge;
  //merge = set_merge;

  double t1 = gettimeofday_sec();
  ARRAY list = merge(arrs, 0, k-1);
  double t2 = gettimeofday_sec();
  std::cout << "merge time: " << t2 - t1 << std::endl;

  // make it successive two-way
  /*
  ARRAY list1 = merge(arrs, 0, k-2);
  ARRAYS arrs2;
  arrs2.push_back(list1);
  arrs2.push_back(arrs[k-1]);
  ARRAY list = merge(arrs2, 0, 1);
  */

  // list must be all sorted
  ///*
  std::cout << "size: " << list.size() << std::endl;
  AITR itr_end = list.end();
  for (AITR itr = list.begin(); itr != itr_end; ++itr) {
    std::cout << *itr << std::endl;
  }
  //*/

  return 0;
}

ARRAY linear_search_merge(ARRAYS &arrs, int from, int to)
{
  AITR itrs[to+1];
  AITR itrs_end[to+1];
  int current[to+1];
  for (int i = from; i <= to; ++i) {
    itrs[i] = arrs[i].begin();
    itrs_end[i] = arrs[i].end();
    current[i] = *(itrs[i]);
  }

  ARRAY sorted_list;
  while (1) {
    bool is_all_done = true;
    for (int i = from; i <= to; ++i) {
      if (itrs[i] != itrs_end[i]) {
        is_all_done = false;
        break;
      }
    }
    if (is_all_done) { break; }
    int smallest = std::numeric_limits<int>::max(); 
    int smallesti = to + 10;
    for (int i = from; i <= to; ++i) {
      if (itrs[i] == itrs_end[i]) { continue; }
      if (current[i] < smallest) {
        smallest = current[i];
        smallesti = i;
      }
    }
    if (smallesti >= from && smallesti <= to) {
      sorted_list.push_back(current[smallesti]);
      ++(itrs[smallesti]);
      current[smallesti] = *(itrs[smallesti]);
    }
  }

  return sorted_list;
}

ARRAY heap_merge(ARRAYS &arrs, int from, int to)
{
  AITR itrs[to+1];
  AITR itrs_end[to+1];
  for (int i = from; i <= to; ++i) {
    itrs[i] = arrs[i].begin();
    itrs_end[i] = arrs[i].end();
    //std::cout << "size-" << i << ": " << arrs[i].size() << std::endl;
  }

  ARRAY sorted_list;
  std::vector<VAL> heap;

  for (int i = from; i <= to; ++i) {
    if (itrs[i] == itrs_end[i]) { continue; }
    heap.push_back(VAL(*(itrs[i]), i));
  }
  make_heap(heap.begin(), heap.end(), greater_VAL());
  do {
    int off = heap.at(0).off;
    sorted_list.push_back(heap.at(0).val);
    ++(itrs[off]);
    pop_heap(heap.begin(), heap.end(), greater_VAL());
    heap.pop_back();
    if (itrs[off] != itrs_end[off]) {
      heap.push_back(VAL(*(itrs[off]), off));
      push_heap(heap.begin(), heap.end(), greater_VAL());
    }
  } while (!heap.empty());

  return sorted_list;
}

ARRAY set_merge(ARRAYS &arrs, int from, int to)
{
  AITR itrs[to+1];
  AITR itrs_end[to+1];
  for (int i = from; i <= to; ++i) {
    itrs[i] = arrs[i].begin();
    itrs_end[i] = arrs[i].end();
  }

  ARRAY sorted_list;

  for (int i = from; i <= to; ++i) {
    while (itrs[i] != itrs_end[i]) {
      std::set<int> sets;
      int base = *(itrs[i]);
      sets.insert(base);

      for (int j = i+1; j <= to; ++j) {
        while (itrs[j] != itrs_end[j]) {
          if (*(itrs[j]) <= base) {
            sets.insert(*(itrs[j]));
            ++(itrs[j]);
          } else {
            break;
          }
        }
      }

      std::set<int>::iterator itr_end = sets.end();
      for (std::set<int>::iterator itr = sets.begin(); itr != itr_end; ++itr) {
        sorted_list.push_back(*itr);
      }
      ++(itrs[i]);
    }
  }

  return sorted_list;
}

void initialize(ARRAYS &arrs)
{
  for (int i = 0; i < k; ++i) {
    int arr_size = size; 
    //int arr_size = size * (1 << i);

    ARRAY arr; 
    for (int j = 0; j < arr_size; ++j) {
      arr.push_back(random() % std::numeric_limits<int>::max()); 
    }
    std::sort(arr.begin(), arr.end());
    AITR itr_end = arr.end();
    /*
    for (AITR itr = arr.begin(); itr != itr_end; ++itr) {
      std::cout << *itr << std::endl;
    }
    std::cout << std::endl;
    */
    arrs.push_back(arr);
  }
}
