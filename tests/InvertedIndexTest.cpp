#include "test_header.h"

#include <Forward.h>

SCENARIO("An inverted index should be created")
{
    GIVEN("A forward index")
    {
        Indexer::Forward::clear();

        std::ifstream file("../data/document1");
        Indexer::Forward::index(file, "document1");

        WHEN("an inverted index is created from the forward index")
        {
        }
    }
}
