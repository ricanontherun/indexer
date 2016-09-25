#ifndef  RICANONTHERUN_FILE_H
#define  RICANONTHERUN_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>

namespace ricanontherun {

#define IS_READ(flags) ( ((flags) == O_RDONLY) || ((flags) & O_RDWR) )

class File {
public:
  enum class READ_STATUS {
    ERROR = -1,
    EXHAUSTED = 0,
    OK = 1
  };

  enum class FILE_STATUS {
    ERROR = -1,
    OK = 0
  };

  enum class ACCESS_ADVICE {
    SEQUENTIAL = POSIX_FADV_SEQUENTIAL,
    RANDOM = POSIX_FADV_RANDOM,
    NOREUSE = POSIX_FADV_NOREUSE,
    WILLNEED = POSIX_FADV_WILLNEED,
    DONTNEED = POSIX_FADV_DONTNEED
  };

  File();
  File(const char *path);
  File(const char *path, int flags);
  File(const char *path, int flags, ACCESS_ADVICE advice);

  ~File();

  bool Ok();
  READ_STATUS Read(ssize_t bytes = BUFSIZ);
  const std::string &Get() const;

private:
  int __fd;
  std::string __buf;
  READ_STATUS __last_read_status;

  ssize_t ReadIntoBuffer(char *buf, ssize_t bytes);
  bool ReadFailed() const;

  void SetReadStatus(READ_STATUS);
  READ_STATUS GetReadStatus() const;
};

} // Namespace File

#endif // RICANONTHERUN_FILE_H
