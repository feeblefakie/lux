#include "searcher.h"
#include <iostream>
#include <string>
#include <time.h>
#include <sys/time.h>

double gettimeofday_sec()
{
  struct timeval tv; 
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

int main(int argc, char *argv[])
{
  char buf[256] = "sony AND vaio";
  if (argc == 2) {
    memset(buf, 0, 256);
    strncpy(buf, argv[1], 256);
  }

  Lux::Dist::Searcher s("blogs");
  Lux::SortCondition scond(Lux::SORT_SCORE, Lux::DESC);
  //Lux::SortCondition scond(Lux::SORT_ATTR_INT, Lux::DESC, "docnum");
  //Lux::SortCondition scond(Lux::SORT_ATTR_STR, Lux::DESC, "docnum");
  Lux::Paging paging(5);
  Lux::Condition cond(scond, paging);

  double t3 = gettimeofday_sec();
  std::string line;
  while (getline(std::cin, line)) {
    double t1 = gettimeofday_sec();
    Lux::ResultSet rs = s.search(line.c_str(), cond); 
    double t2 = gettimeofday_sec();
    // for each search result
    std::cout << "total hits: " << rs.get_total_num() << " - " << t2-t1 << "(s)" << std::endl;
  }
  double t4 = gettimeofday_sec();
  std::cout << "total time: " << t4-t3 << "(s)" << std::endl;

  return 0;
}
