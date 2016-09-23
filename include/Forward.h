#ifndef INVERTED_INDEX_FOWARDINDEX_H
#define INVERTED_INDEX_FOWARDINDEX_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <mutex>

#include <DocumentRepository.h>

namespace Indexer {

typedef std::unordered_map<
    docID,
    std::unordered_set<std::string>
> ForwardIndex_T;

class Forward {
public:
  static void index(std::ifstream &input, docID id);
  static const ForwardIndex_T &data();
  static void clear();

private:
  static std::mutex index_lock;
  static ForwardIndex_T __index;
  static void Tokenize(const std::string & context, std::vector<std::string> &tokens);
  static void Sanitize(std::string & line);
};

} // End Indexer
#endif
