#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <Forward.h>

std::mutex file_queue_lock;

class Worker {
public:
  Worker() {}

  void operator()() {
      /*
    std::string chunk_path;

    // TODO: How can we use lock_guards in this context?
    file_queue_lock.lock();

    // START CRITICAL SECTION
    while (!file_queue.empty()) {
      chunk_path = file_queue.front();
      file_queue.pop();
      // END CRITICAL SECTION

      file_queue_lock.unlock();

      std::string file_path(chunk_path);
      std::ifstream file(file_path);

      if (file.good()) {
        Indexer::Forward::index(file, chunk_path);
      } else {
        std::cerr << "Failed to open file: " << file_path << "\n";
      }
    }
    */
  }

};

void populate_file_queue(
    const std::string &directory_path,
    std::queue<std::string> &file_queue
) {
  DIR *dir = opendir(directory_path.c_str());
  struct dirent *file;

  if (dir == NULL) {
    std::cerr << "Failed to open directory '" << directory_path << "' \n";
    return;
  }

  while ((file = readdir(dir)) != NULL) {
    if (file->d_type != DT_REG) { // Ignore non-regular files.
      continue;
    }

    file_queue.push(directory_path + "/" + std::string(file->d_name)); // Throw it on the queue.
  }
}

void index()
{
  /*
  // Take-away: Spawning more worker threads than there are files
  // apparently causes issues. Research!
  unsigned int max_threads = std::thread::hardware_concurrency();

  // We definitely don't want to spawn more worker threads
  // than there are files.
  unsigned int threads = std::min(max_threads, static_cast<unsigned int>(file_queue.size()));

  std::vector<std::thread> workers(threads);

  // Create a series of worker threads
  for (int i = 0; i < threads; i++) {
    workers.push_back(std::thread(Worker()));
  }

  for (auto &worker : workers) {
    // TODO: Research. What kinds of situations can cause a
    // thread to no longer be joinable?
    if (worker.joinable()) {
      worker.join();
    }
  }
  */
}

void partition_file(const std::string & file_path)
{
    // Rather than doing a shit ton of system, call a script.
}

void index_file(const std::string & file_path)
{
    partition_file(file_path);
    // escape directory separators or something?
    // split the file into chunks at /tmp/file_path.chunks
    std::cout << "Indexing file '" << file_path << "' \n";
}

void index_directory(const char *directory)
{
    std::queue<std::string> file_queue;
    populate_file_queue(directory, file_queue);

    std::string file;

    while ( !file_queue.empty() ) {
        file = file_queue.front();

        index_file(file);

        file_queue.pop();
    }
}

int main(int argc, char **argv) {
  if ( argc == 1 ) {
    // Display help.
  }

  const char *file_or_directory = argv[1];
  struct stat file_stat;
  int stat_ret = stat(file_or_directory, &file_stat);

  if ( stat_ret == -1 ) {
    std::cerr << "Failed to open file '" << file_or_directory << "'\n";
    return EXIT_FAILURE;
  }

  mode_t file_mode = file_stat.st_mode;

  if (S_ISDIR(file_mode)) {
    index_directory(file_or_directory);
  } else if (S_ISREG(file_mode)) {
    index_file(std::string(file_or_directory));
  } else {
    std::cerr << "Unsupported file type \n";
  }

  return EXIT_SUCCESS;
}
