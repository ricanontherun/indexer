#include "test_header.h"

#include <indexer/DocumentRepository.h>

TEST_CASE("Document Repository Tests", "[repo]")
{
    SECTION("It can set things")
    {

        Indexer::docID doc1_id = Indexer::DocumentRepository::getDocID("/path/to/document1");
        Indexer::docID doc2_id = Indexer::DocumentRepository::getDocID("/path/to/document2");
        Indexer::docID doc3_id = Indexer::DocumentRepository::getDocID("/path/to/document3");
        Indexer::docID doc4_id = Indexer::DocumentRepository::getDocID("/path/to/document4");

        auto repo = Indexer::DocumentRepository::data();

        REQUIRE(repo.count("/path/to/document1") == 1);
        REQUIRE(repo.find("/path/to/document1")->second == doc1_id);

        REQUIRE(repo.count("/path/to/document2") == 1);
        REQUIRE(repo.find("/path/to/document2")->second == doc2_id);

        REQUIRE(repo.count("/path/to/document3") == 1);
        REQUIRE(repo.find("/path/to/document3")->second == doc3_id);

        REQUIRE(repo.count("/path/to/document4") == 1);
        REQUIRE(repo.find("/path/to/document4")->second == doc4_id);
    }

    SECTION("It knows how to lookup an existing document")
    {
        Indexer::docID doc_id = Indexer::DocumentRepository::getDocID("/path/to/document4");
        Indexer::docID doc_id_again = Indexer::DocumentRepository::getDocID("/path/to/document4");

        auto repo = Indexer::DocumentRepository::data();

        REQUIRE(repo.count("/path/to/document4") == 1);

        REQUIRE(repo.find("/path/to/document4")->second == doc_id);
        REQUIRE(repo.find("/path/to/document4")->second == doc_id_again);
    }
}
