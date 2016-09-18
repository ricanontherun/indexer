#include <Forward.h>

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace Indexer {

FowardIndex_T Forward::__index;
std::mutex Forward::index_lock;

void Forward::index(std::ifstream &file, std::string key) {
  std::string line;
  std::vector<std::string> tokens;

  while (std::getline(file, line)) {
    // Convert string to lowercase.
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    // Tokenize.
    Forward::Tokenize(line, tokens);

    Forward::index_lock.lock();
    Forward::__index[key].insert(tokens.begin(), tokens.end());
    Forward::index_lock.unlock();

    tokens.clear();
  }
}

void Forward::clear()
{
    Forward::__index.clear();
}

const FowardIndex_T &Forward::data() {
  return Forward::__index;
}

void Forward::Tokenize(std::string context, std::vector<std::string> &tokens) {
  boost::split(tokens, context, boost::is_any_of(" "), boost::token_compress_on);
}

}
