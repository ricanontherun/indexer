#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <fstream>

#include <Forward.h>

std::mutex file_queue_lock;
std::queue<std::string> file_queue;

class Worker {
public:
  Worker() {}

  void operator()() {
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
  }

};

void fill_queue(std::queue<std::string> &file_queue) {
  std::string path("/tmp/file.chunks");
  DIR *dir = opendir(path.c_str());
  struct dirent *file;

  if (dir == NULL) {
    std::cerr << "Failed to open directory data\n";
    return;
  }

  while ((file = readdir(dir)) != NULL) {
    if (file->d_type != DT_REG) { // Ignore non-regular files.
      continue;
    }

    file_queue.push(path + "/" + std::string(file->d_name)); // Throw it on the queue.
  }
}

int main(int argc, char **argv) {
  // File queue.
  fill_queue(file_queue);

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

  return 0;
}
