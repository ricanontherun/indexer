#ifndef INDEXER_INVERTED_H
#define INDEXER_INVERTED_H

#include <unordered_map>
#include <set>

#include <Forward.h>
#include <DocumentRepository.h> // Might wanna pull the typedefs into a types.h ...

namespace Indexer
{

typedef std::unordered_map<
    std::string, // Token
    std::set<docID> // List of occurence documents
> InvertedIndex_T;

class Inverted
{
    public:
        static void index(const ForwardIndex_T & index);
    private:
        static InvertedIndex_T __index;

};

} // Namespace Indexer

#endif
