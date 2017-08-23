#include <indexer/Forward.h>

#include <iostream>

#include <boost/algorithm/string.hpp>


namespace Indexer {

ForwardIndex_T Forward::__index;
std::mutex Forward::index_lock;

void Forward::index(File &file, docID id) {
  std::string line;
  std::vector<std::string> tokens;

  while (file.Read() == File::READ_STATUS::OK) {
    line = file.Get();
    Forward::Sanitize(line);
    Forward::Tokenize(line, tokens);

    Forward::index_lock.lock();
    Forward::__index[id].insert(tokens.begin(), tokens.end());
    Forward::index_lock.unlock();

    tokens.clear();
  }
}

void Forward::clear() {
  Forward::__index.clear();
}

const ForwardIndex_T &Forward::data() {
  return Forward::__index;
}

void Forward::Tokenize(const std::string &context, std::vector<std::string> &tokens) {
  // TODO: Ideally this tokenization would just extract words..maybe boost is a bad idea for this.
  boost::split(tokens, context, boost::is_any_of(" "), boost::token_compress_on);
}

void Forward::Sanitize(std::string & line)
{
  // Remove all non-alphanumeric spaces and non-whitespace
  line.erase(
      std::remove_if(line.begin(), line.end(), [](char c) {
        return !(std::isalpha(c) || std::isspace(c));
      }),
      line.end()
  );

  // TOOD: This is going to provide inaccurate results...they might be searching for
  // mixed case tokens.
  std::transform(line.begin(), line.end(), line.begin(), ::tolower);
}

}
