#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

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

  closedir(dir);
}

void index() {
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

bool partition_file(const std::string &file_path, std::string &out_tmp) {
  // Construct and execute the partition command
  std::string command = "sh ../scripts/partition_file.sh " + file_path;
  FILE *fp = popen(command.c_str(), "r");

  if (!fp) {
    return false;
  }

  // Read the script's output.
  char buf[BUFSIZ];
  fgets(buf, BUFSIZ, fp);

  // Capture the return code for error handling.
  int ret = pclose(fp);

  if (ret == -1) {
    return false;
  }

  out_tmp = std::string(buf);
  out_tmp.erase(std::remove(out_tmp.begin(), out_tmp.end(), '\n'), out_tmp.end());

  return true;
}

void index_file(const std::string &file_path) {
  std::string chunks_dir;

  if (!partition_file(file_path, chunks_dir)) {
    std::cerr << "Failed to partition file.\n";
    std::exit(EXIT_FAILURE);
  }

  std::queue<std::string> chunk_queue;
  populate_file_queue(chunks_dir, chunk_queue);

  std::string chunk_path;
  while (!chunk_queue.empty()) {
    chunk_path = chunk_queue.front();
    chunk_queue.pop();

    std::cout << "Processing chunk " << chunk_path << "\n";
  }
  // Use worker threads to process each chunk and store
  // the data at key file_path.
}

void index_directory(const char *directory) {
  std::queue<std::string> file_queue;
  populate_file_queue(directory, file_queue);

  std::string file;

  while (!file_queue.empty()) {
    file = file_queue.front();
    file_queue.pop();

    index_file(file);
  }
}

int main(int argc, char **argv) {
  if (argc == 1) {
    // Display help.
  }

  const char *file_or_directory = argv[1];
  struct stat file_stat;
  int stat_ret = stat(file_or_directory, &file_stat);

  if (stat_ret == -1) {
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
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
