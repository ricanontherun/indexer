#ifndef INDEXER_INVERTED_H
#define INDEXER_INVERTED_H

#include <unordered_map>

#include <Forward.h>

namespace Indexer
{

class Inverted
{
    public:
        static void index(const Forward & forward_index);
    private:
        std::unordered_map<
            std::string, // Token
            std::vector<std::string> // List of occurence documents
        > __index;

};

} // Namespace Indexer

#endif
