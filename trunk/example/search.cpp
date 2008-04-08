#include <lux/search.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cerr << argv[0] << " query" << std::endl;
        exit(1);
    }

    // define document definition
    Lux::DocumentDefinition dd("blog");
    static Lux::FieldDefinition title_fd("title");
    title_fd.set_indexed(true);
    title_fd.set_displayed(true);
    dd.add(title_fd);
  
    // create search condition
    Lux::Condition cond(Lux::SORT_SCORE_DESC);

    // search for the query
    Lux::Searcher *s = new Lux::Searcher("./storage", dd);
    Lux::ResultSet rs = s->search(argv[1], cond); 

    // for each search result
    std::cout << "total hits: " << rs.get_total_num() << std::endl;
    rs.init_iter();
    while (rs.has_next()) {
      Lux::Result r = rs.get_next();
      std::cout << "[id] " << r.get_id() << std::endl;
      std::cout << "[title] " << r.get("title") << std::endl;
      std::cout << std::endl;
    }
    delete s;

    return 0;
}
