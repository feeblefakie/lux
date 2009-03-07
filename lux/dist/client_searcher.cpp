#include "searcher.h"
#include <iostream>

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

  Lux::ResultSet rs = s.search(buf, cond);

  std::cout << "total hits: " << rs.get_total_num() << std::endl;
  std::cout << "base: " << rs.get_base() << std::endl;
  std::cout << "num: " << rs.get_num() << std::endl;
  rs.init_iter();
  while (rs.has_next()) {
    Lux::Result r = rs.get_next();
    std::cout << "[id] " << r.get_id() << std::endl;
    std::cout << "[title] " << r.get("title") << std::endl;
    //std::cout << "[content] " << r.get("content") << std::endl;
    std::cout << "[score] " << r.get_score() << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
