#ifndef INVERTED_INDEX_FOWARDINDEX_H
#define INVERTED_INDEX_FOWARDINDEX_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <mutex>

namespace Indexer {

typedef std::unordered_map<
    std::string,
    std::unordered_set<std::string>
> FowardIndex_T;

class Forward {
private:
  static std::mutex index_lock;
  static FowardIndex_T __index;
  static void Tokenize(std::string context, std::vector<std::string> &tokens);

public:
  static void index(std::ifstream &input, std::string key);
  static const FowardIndex_T &data();
};

} // End Indexer
#endif
