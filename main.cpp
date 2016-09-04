#include <iostream>
#include <ForwardIndex.h>

int main() {

  std::ifstream file("/home/ricanontherun/Code/indexer/document1");

  Indexer::ForwardIndex::index(file, "jawn");

  return 0;
}