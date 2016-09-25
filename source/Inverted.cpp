#include <indexer/Inverted.h>
#include <json.hpp>

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

  index_json["documents"] = DocumentRepository::data();
  index_json["index"] = Inverted::__index;

  std::ofstream output_file(output_path, std::ios::out);

  if ( !output_file.good() ) {
    std::cerr << "Failed to open output file '" << output_path<< "'\n";
    return false;
  }

  // TODO: Improve efficiency...
  output_file << index_json.dump(2);
  output_file.close();

  return true;
}

} // Namespace Indexer
