#include <iostream>
#include <ForwardIndex.h>

int main() {

  std::ifstream file("/home/ricanontherun/Code/indexer/document1");

  Indexer::ForwardIndex::index(file, "jawn");

  auto index = Indexer::ForwardIndex::data();

  for ( auto const & word : index["jawn"] ) {
    std::cout << word << "\n";
  }

  return 0;
}