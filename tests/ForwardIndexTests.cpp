#define CATCH_CONFIG_MAIN

#include "test_header.h"
#include <algorithm>

#include "ForwardIndex.h"

TEST_CASE("ForwardIndex will index a document into it's unique words", "[ForwardIndex], [unit]") {
  SECTION("yep") {
    std::unordered_set<std::string> expected{
        "asd"
    };

    std::ifstream file("../data/document1");
    Indexer::ForwardIndex::index(file, "document1");

    Indexer::FowardIndex_T actual = Indexer::ForwardIndex::data();
    std::unordered_set<std::string> data = actual["document1"];

    std::vector<std::string> difference;
  }
}

