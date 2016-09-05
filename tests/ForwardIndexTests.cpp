#define CATCH_CONFIG_MAIN

#include "test_header.h"
#include <set>
#include <unordered_set>
#include <iterator>
#include "Forward.h"

TEST_CASE("ForwardIndex will index a document into it's unique words", "[ForwardIndex], [unit]") {
  SECTION("yep") {
    std::set<std::string> expected{
        "mary", "had", "a", "little", "lamb", "his", "fur", "was", "white", "as", "snow"
    };

    std::ifstream file("../data/document1");
    Indexer::Forward::index(file, "document1");

    Indexer::FowardIndex_T actual = Indexer::Forward::data();
    std::unordered_set<std::string> data = actual["document1"];
    std::set<std::string> ordered_data(data.begin(), data.end());

    std::vector<std::string> difference;

    std::set_difference(ordered_data.begin(),
                        ordered_data.end(),
                        expected.begin(),
                        expected.end(),
                        std::inserter(difference, difference.begin()));

    REQUIRE(difference.size() == 0);
  }
}

