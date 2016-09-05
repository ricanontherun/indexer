#include <Forward.h>

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Indexer {

FowardIndex_T Forward::document_words;

void Forward::index(std::ifstream &file, std::string key) {
  std::string line;
  std::vector<std::string> tokens;

  while (std::getline(file, line)) {
    // Convert string to lowercase.
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    // Tokenize.
    Forward::Tokenize(line, tokens);

    // We let the unordered_set weed out any duplicate words.
    Forward::document_words[key].insert(tokens.begin(), tokens.end());

    tokens.clear();
  }
}

const FowardIndex_T &Forward::data() {
  return Forward::document_words;
}

void Forward::Tokenize(std::string context, std::vector<std::string> &tokens) {
  boost::split(tokens, context, boost::is_any_of(" "), boost::token_compress_on);
}

}