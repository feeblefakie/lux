#include <lux/search.h>
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
  if (argc < 3) {
      std::cerr << argv[0] << " service query" << std::endl;
      exit(1);
  }
  double t1, t2;

  t1 = gettimeofday_sec();

  Lux::Config::Document doc_config;
  Lux::DocumentConfigParser::parse(argv[1], doc_config);

  // create search condition
  Lux::SortCondition scond(Lux::SORT_SCORE, Lux::DESC);
  Lux::Paging paging(5);
  Lux::Condition cond(scond, paging);

  // search for the query
  Lux::Engine engine(doc_config);
  if (!engine.open(argv[1], Lux::DB_RDONLY)) {
    std::cerr << "engine.open failed" << std::endl;
    exit(-1);
  }
  Lux::Searcher searcher(engine);
  
  Lux::ResultSet rs = searcher.search(argv[2], cond); 
  t2 = gettimeofday_sec();

  // for each search result
  std::cout << "total hits: " << rs.get_total_num() << std::endl;
  std::cout << "base: " << rs.get_base() << std::endl;
  std::cout << "num: " << rs.get_num() << std::endl;
  std::cout << "time: " << t2 - t1 << std::endl;
  rs.init_iter();
  while (rs.has_next()) {
    Lux::Result r = rs.get_next();
    std::cout << "[id] " << r.get_id() << std::endl;
    std::cout << "[title] " << r.get("title") << std::endl;
    //std::cout << "[content] " << r.get("content") << std::endl;
    std::cout << "[created_at] " << r.get("created_at") << std::endl;
    std::cout << "[url] " << r.get("url") << std::endl;
    std::cout << "score: " << r.get_score() << std::endl;
    std::cout << std::endl;
  }

  return 0;
}
