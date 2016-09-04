#ifndef INVERTED_INDEX_FOWARDINDEX_H
#define INVERTED_INDEX_FOWARDINDEX_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>

namespace Indexer {

typedef std::unordered_map<
    std::string,
    std::unordered_set<std::string>
> forward_index_t;

class ForwardIndex {
private:
  static forward_index_t document_words;

  static void Tokenize(std::string context, std::vector<std::string> &tokens);

public:
  static void index(std::ifstream &input, std::string key);
  static const forward_index_t &data();
};

} // End Indexer
#endif
