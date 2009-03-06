#include <lux/index.h>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>

//記事IDブログID投稿日URLタイトル本文

int main(int argc, char *argv[])
{
  if (argc != 2) {
      std::cerr << argv[0] << " service" << std::endl;
      exit(1);
  }

  Lux::Config::Document doc_config;
  if (!Lux::DocumentConfigParser::parse(argv[1], doc_config)) {
    std::cerr << "parse failed." << std::endl;
    exit(1);
  }

  // create indexer
  Lux::Engine engine(doc_config);
  if (!engine.open(argv[1], Lux::DB_CREAT)) {
    std::cerr << "opening engine failed" << std::endl;
    exit(-1);
  }
  Lux::Indexer indexer(engine);

  // for each document to be indexed
  int i = 0;
  std::string line;
  while (getline(std::cin, line)) {

    std::vector<std::string> items;
    boost::split(items, line, boost::is_any_of(""));

    // invalid input
    if (items.size() != 6) { continue; }

    // create a document and add a title field object to it
    Lux::Document *doc = new Lux::Document(items[0]);
    doc->add(Lux::Field::create("url", items[3]));
    doc->add(Lux::Field::create("title", items[4]));
    doc->add(Lux::Field::create("content", items[5]));
    doc->add(Lux::Field::create("created_at", items[2]));

    // add the created document to index
    indexer.add(doc);
    std::cout << ++i << ": " << items[0] << " indexed." << std::endl;

    delete doc;
  }

  return 0;
}
