#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <errno.h>
#include <string.h>

#include <DocumentRepository.h>
#include <Forward.h>
#include <Inverted.h>
#include <types.h>

std::mutex file_queue_lock;

unsigned int max_threads = std::thread::hardware_concurrency();

/**
 * Populate a queue with relative paths to the REGULAR
 * files in a directory.
 *
 * @param directory_path
 * @param file_queue
 */
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

/**
 * Split a file into small chunks,
 * the tmp directory which contains them is returned via out_tmp
 *
 * @param file_path
 * @param out_tmp
 *
 * @return
 */
bool split_file(const std::string &file_path, std::string &out_tmp) {
  // Construct and execute the split script
  std::string command = "sh ../scripts/split.sh 64k " + file_path;
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

void dispatch_file_workers(std::queue<std::string> &file_queue, Indexer::docID doc_id) {
  // We definitely don't want to spawn more worker threads
  // than there are files.
  unsigned int threads = std::min(max_threads, static_cast<unsigned int>(file_queue.size()));

  std::vector<std::thread> workers(threads);

  // Create a series of worker threads
  for (int i = 0; i < threads; i++) {
    workers.push_back(std::thread([&file_queue, doc_id]() {
      std::string chunk_path;

      file_queue_lock.lock();

      while (!file_queue.empty()) {
        chunk_path = file_queue.front();
        file_queue.pop();

        file_queue_lock.unlock();

        std::ifstream file(chunk_path);

        if (file.good()) {
          Indexer::Forward::index(file, doc_id);
          file.close();
        } else {
          std::cerr << "Failed to open file: " << chunk_path << "\n";
        }
      }
    }));
  }

  for (auto &worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

/**
 * Index a particular file.
 *
 * @param file_path
 */
void index_file(const std::string &file_path) {
  std::string chunks_dir;

  if (!split_file(file_path, chunks_dir)) {
    std::cerr << "Failed to split file.\n";
    std::exit(EXIT_FAILURE);
  }

  std::cout << "Indexing " << file_path << " using up to " << max_threads << " threads...\n";

  std::queue<std::string> chunk_queue;
  populate_file_queue(chunks_dir, chunk_queue);

  dispatch_file_workers(
      chunk_queue,
      Indexer::DocumentRepository::getDocID(file_path)
  );
}

/**
 * Index all the files in a given directory.
 *
 * @param directory
 */
void index_directory(const std::string &directory) {
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
  if (argc != 3) {
    // Display help.
    std::cout << "Usage: indexer path/to/input path/to/output";
    return EXIT_FAILURE;
  }

  const char *file_or_directory = argv[1];
  struct stat file_stat;
  int stat_ret = stat(file_or_directory, &file_stat);

  if (stat_ret == -1) {
    std::cerr << strerror(errno) << " '" << file_or_directory << "'\n";
    return EXIT_FAILURE;
  }

  mode_t file_mode = file_stat.st_mode;

  if (S_ISDIR(file_mode)) {
    index_directory(std::string(file_or_directory));
  } else if (S_ISREG(file_mode)) {
    index_file(std::string(file_or_directory));
  } else {
    std::cerr << "Unsupported file type \n";
    return EXIT_FAILURE;
  }

  // Convert to inverted index
  Indexer::Inverted::index(Indexer::Forward::data());

  std::string output_path(argv[2]);

  std::cout << "Writing to index file " << output_path << "\n";
  Indexer::Inverted::dump(output_path);

  return EXIT_SUCCESS;
}
