#include <Inverted.h>

#include <iostream>

namespace Indexer
{

InvertedIndex_T Inverted::__index;

void Inverted::index(const ForwardIndex_T & index)
{
    for ( auto & kv : index ) {
        for ( auto & term : kv.second ) {
            Inverted::__index[term].insert(kv.first);
        }
    }
}

} // Namespace Indexer
