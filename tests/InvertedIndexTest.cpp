#include "test_header.h"

#include <indexer/Forward.h>
#include <indexer/Inverted.h>

SCENARIO("An inverted index should be created")
{
    GIVEN("A forward index")
    {
        Indexer::Forward::clear();

        auto doc_id = Indexer::DocumentRepository::getDocID("../data/document1");

        std::ifstream file("../data/document1");

        Indexer::Forward::index(file, doc_id);

        file.close();

        WHEN("an inverted index is created from the forward index")
        {
            Indexer::Inverted::index(Indexer::Forward::data());
        }
    }

    GIVEN("A file to be indexed")
    {
    }
}
