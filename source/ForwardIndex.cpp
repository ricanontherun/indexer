#include <ForwardIndex.h>

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Indexer {

FowardIndex_T ForwardIndex::document_words;

void ForwardIndex::index(std::ifstream &file, std::string key) {
  std::string line;
  std::vector<std::string> tokens;

  while (std::getline(file, line)) {
    // Convert string to lowercase.
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    // Tokenize.
    ForwardIndex::Tokenize(line, tokens);

    // We let the unordered_set weed out any duplicate words.
    ForwardIndex::document_words[key].insert(tokens.begin(), tokens.end());

    tokens.clear();
  }
}

const FowardIndex_T &ForwardIndex::data() {
  return ForwardIndex::document_words;
}

void ForwardIndex::Tokenize(std::string context, std::vector<std::string> &tokens) {
  boost::split(tokens, context, boost::is_any_of(" "), boost::token_compress_on);
}

}