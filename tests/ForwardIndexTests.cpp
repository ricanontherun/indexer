#define CATCH_CONFIG_MAIN

#include "test_header.h"
#include <set>
#include <unordered_set>
#include <iterator>
#include "Forward.h"

TEST_CASE("Forward Index tests", "[ForwardIndex], [unit]") {
  SECTION("The output from the indexer is predictable.") {
    std::set<std::string> expected{
        "mary", "had", "a", "little", "lamb", "his", "fur", "was", "white", "as", "snow"
    };

    std::ifstream file("../data/document1");
    Indexer::Forward::index(file, "document1");

    Indexer::FowardIndex_T actual = Indexer::Forward::data();
    std::unordered_set<std::string> data = actual["document1"];
    std::set<std::string> ordered_data(data.begin(), data.end());

    REQUIRE(expected == ordered_data);
  }

  SECTION("The output consists of unique words") {
    std::set<std::string> expected{
        "these", "some", "are", "unique", "words"
    };

    std::ifstream file("../data/duplicate_data");
    Indexer::Forward::index(file, "duplicate_data");

    Indexer::FowardIndex_T actual = Indexer::Forward::data();
    std::unordered_set<std::string> data = actual["duplicate_data"];
    std::set<std::string> ordered_data(data.begin(), data.end());

    REQUIRE(expected == ordered_data);
  }
}

