#include <Inverted.h>
#include <libs/json.hpp>

#include <iostream>

namespace Indexer {

InvertedIndex_T Inverted::__index;

void Inverted::index(const ForwardIndex_T &index) {
  for (auto &kv : index) {
    for (auto &term : kv.second) {
      Inverted::__index[term].insert(kv.first);
    }
  }
}

bool Inverted::dump(const std::string &output_path) {
  using json = nlohmann::json;

  // Create a json object
  json index_json;

  // Transform the __index object into a the json object
  index_json["thing"] = {"asasdasd", "hello"};
  // Output the file.

  std::cout << index_json;

  return true;
}

} // Namespace Indexer
