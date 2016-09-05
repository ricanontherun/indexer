#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>

std::mutex file_queue_lock;
std::queue<std::string> file_queue;

class Worker {
public:
  Worker() {}

  void operator()() {
    std::string file_name;

    // TODO: How can we use lock_guards in this context?
    file_queue_lock.lock();

    // START CRITICAL SECTION
    while (!file_queue.empty()) {
      file_name = file_queue.front();
      file_queue.pop();
      // END CRITICAL SECTION

      file_queue_lock.unlock();

      std::cout << "Processing file: ./data/" << file_name << "\n";
    }
  }

};

void fill_queue(std::queue<std::string> &file_queue) {
  DIR *dir = opendir("/home/ricanontherun/Code/indexer/data");
  struct dirent *file;

  while ((file = readdir(dir)) != NULL) {
    if (file->d_type != DT_REG) { // Ignore non-regular files.
      continue;
    }

    file_queue.push(std::string(file->d_name)); // Throw it on the queue.
  }
}

int main() {
  // File queue.
  fill_queue(file_queue);

  // Take-away: Spawning more worker threads than there are files
  // apparently causes issues. Research!
  unsigned int max_threads = std::thread::hardware_concurrency();
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
  // which iterate over a directory of files.
  // Each thread is responsible for calling index on a ifstream
  // which will add a document key/value to the static index.
  return 0;
}