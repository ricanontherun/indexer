#include <thread>
#include <queue>
#include <dirent.h>
#include <iostream>
#include <mutex>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <algorithm>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <indexer/DocumentRepository.h>
#include <indexer/Forward.h>
#include <indexer/Inverted.h>

#define IS_SELF(dir) (strcmp((dir->d_name), ".") == 0)
#define IS_PARENT(dir) (strcmp((dir->d_name), "..") == 0)

std::mutex file_queue_lock;

unsigned int max_threads = std::thread::hardware_concurrency();
std::size_t file_queue_pos = 0;

/**
 * Populate a queue with relative paths to the REGULAR
 * files in a directory.
 *
 * @param directory_path
 * @param file_queue
 */
void populate_file_queue(
    const std::string &directory_path,
    std::vector<std::string> &file_queue
) {
  DIR *dir = opendir(directory_path.c_str());
  struct dirent *file;

  if (dir == NULL) {
    perror("populate_file_queue");
    return;
  }

  while ((file = readdir(dir)) != NULL) {
      // Check for '.' and '..'
      if ( IS_SELF(file) || IS_PARENT(file) ) {
        continue;
      }

      std::string path(directory_path);

      if (file->d_type == DT_REG) {
        file_queue.push_back(path.append("/").append(file->d_name));
        continue;
      }

    populate_file_queue(path.append(file->d_name), file_queue);
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
  // TODO: There really should be a better way of doing this. This requires the split
  // file command to be globally accessible.
  std::string command = "split_file.sh 64k " + file_path;
  FILE *fp = popen(command.c_str(), "r");

  if (fp == NULL) {
      perror("popen");
    return false;
  }

  // Read the script's output.
  char buf[BUFSIZ];
  fgets(buf, BUFSIZ, fp);

  // Capture the return code for error handling.
  int ret = pclose(fp);

  if (ret == -1) {
      perror("pclose");
    return false;
  }

  out_tmp = std::string(buf);
  out_tmp.erase(std::remove(out_tmp.begin(), out_tmp.end(), '\n'), out_tmp.end());

  return true;
}

void dispatch_file_workers(std::vector<std::string> &file_queue, Indexer::docID doc_id) {
    // TODO: Check threading logic.
  unsigned int threads = std::min(max_threads, static_cast<unsigned int>(file_queue.size()));
  std::size_t queue_pos = 0;
  std::size_t queue_size = file_queue.size();
  std::vector<std::thread> workers(threads);

  // Create a series of worker threads
  for (int i = 0; i < threads; i++) {
    workers.push_back(std::thread([&file_queue, doc_id, &queue_pos, queue_size]() {
      std::string chunk_path;

      file_queue_lock.lock();

      while (queue_pos < queue_size) {
        chunk_path = file_queue.at(queue_pos++);

        file_queue_lock.unlock();

        File f(chunk_path.c_str(), O_RDONLY, File::ACCESS_ADVICE::SEQUENTIAL);

        if (f.Ok()) {
          Indexer::Forward::index(f, doc_id);
        } else {
          std::cerr << "Failed to open file '" << chunk_path << "\n";
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
      exit(EXIT_FAILURE);
  }

  std::cout << "Indexing " << file_path << " using up to " << max_threads << " threads...\n";

  std::vector<std::string> chunk_queue;
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
  std::vector<std::string> file_queue;

  populate_file_queue(directory, file_queue);

  for (const auto & file : file_queue) {
    std::cout << file << "\n";
  }

  std::size_t file_queue_size = file_queue.size();

  std::string file;

  while (file_queue_pos < file_queue_size) {
    file = file_queue.at(file_queue_pos++); // Get the next element and move the pointer forward.

    index_file(file);
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    // Display help.
    std::cout << "Usage: indexer path/to/input path/to/output\n";
    return EXIT_FAILURE;
  }

  const char *file_or_directory = argv[1];
  struct stat file_stat;
  int stat_ret = stat(file_or_directory, &file_stat);

  if (stat_ret == -1) {
      perror("stat");
    return EXIT_FAILURE;
  }

  mode_t file_mode = file_stat.st_mode;

  if (S_ISDIR(file_mode)) {
    index_directory(std::string(file_or_directory));
  } else if (S_ISREG(file_mode)) {
    index_file(std::string(file_or_directory));
  } else {
    std::cerr << "Unsupported file type\n";
    return EXIT_FAILURE;
  }

  // Convert to inverted index
  Indexer::Inverted::index(Indexer::Forward::data());

  std::string output_path(argv[2]);

  std::cout << "Writing to index file " << output_path << "\n";
  Indexer::Inverted::dump(output_path);

  return EXIT_SUCCESS;
}
