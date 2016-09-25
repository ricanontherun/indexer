#ifndef INDEXER_INVERTED_H
#define INDEXER_INVERTED_H

#include <unordered_map>
#include <set>

#include <indexer/Forward.h>
#include <indexer/DocumentRepository.h> // Might wanna pull the typedefs into a types.h ...

namespace Indexer {

typedef std::unordered_map<
    std::string, // Token
    std::set<docID> // List of occurrence documents
> InvertedIndex_T;

class Inverted {
public:
  static void index(const ForwardIndex_T &index);
  static bool dump(const std::string & output_path);
private:
  static InvertedIndex_T __index;

};

} // Namespace Indexer

#endif
