#include <iostream>
#include <Forward.h>

int main() {

  std::ifstream file("/home/ricanontherun/Code/indexer/document1");

  Indexer::Forward::index(file, "jawn");

  auto index = Indexer::Forward::data();

  for ( auto const & word : index["jawn"] ) {
    std::cout << word << "\n";
  }

  return 0;
}