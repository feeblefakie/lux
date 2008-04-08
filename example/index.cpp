#include <lux/index.h>
#include <iostream>
#include <string>

int main(void)
{
  // define document definition 
  Lux::DocumentDefinition dd("blog");
  static Lux::FieldDefinition title_fd("title");
  title_fd.set_indexed(true);
  title_fd.set_displayed(true);
  dd.add(title_fd);
  
  // create indexer
  Lux::Indexer *indexer = new Lux::Indexer("./storage", dd);

  // for each document to be indexed
  std::string line;
  while (getline(std::cin, line)) {
    // split a line into url(id) and title 
    std::string::size_type idx = line.find("");
    if (idx == std::string::npos) {
        continue;
    }
    std::string id = line.substr(0, idx);

    // create a document and add a title field object to it
    Lux::Document *doc = new Lux::Document(id);
    doc->add(Lux::Field::create("title", line.substr(idx + 1), &title_fd));

    // add the created document to index
    indexer->add(*doc);
    std::cout << id << " indexed." << std::endl;

    delete doc;
  }
  delete indexer;

  return 0;
}
