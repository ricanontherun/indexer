#include "file.h"

namespace ricanontherun {

File::File() {}

File::File(const char *path) {
  this->__fd = open(path, O_RDONLY);
}

File::File(const char *path, int flags) : __fd(open(path, flags)) {
}

File::File(const char *path, int flags, ACCESS_ADVICE advice) : __fd(open(path, flags)) {
  if (IS_READ(flags)) {
    posix_fadvise(this->__fd, 0, 0, static_cast<int>(advice));
  }
}

File::~File() {
  if (this->Ok()) {
    close(this->__fd);
  }
}

bool File::Ok() {
  return this->__fd != static_cast<int>(FILE_STATUS::ERROR);
}

File::READ_STATUS File::Read(ssize_t bytes) {
  char buf[bytes + 1];

  ssize_t bytes_read = this->ReadIntoBuffer(buf, bytes);

  if (!this->ReadFailed()) {
    // Top off the string at however many bytes were actually read.
    buf[bytes_read] = '\0';

    // Set the internal buffer.
    this->__buf = std::string(buf);
  }

  return this->GetReadStatus();
}

const std::string &File::Get() const {
  return this->__buf;
}

ssize_t File::ReadIntoBuffer(char *buf, ssize_t bytes) {
  ssize_t bytes_read = read(this->__fd, buf, bytes);

  // Set the read status.
  switch (bytes_read) {
  case -1:this->__last_read_status = READ_STATUS::ERROR;
    break;
  case 0:this->__last_read_status = READ_STATUS::EXHAUSTED;
    break;
  default:this->__last_read_status = READ_STATUS::OK;
    break;
  }

  return bytes_read;
}

void File::SetReadStatus(READ_STATUS status) {
  this->__last_read_status = status;
}

File::READ_STATUS File::GetReadStatus() const {
  return this->__last_read_status;
}

bool File::ReadFailed() const {
  return this->__last_read_status == READ_STATUS::ERROR;
}

} // Namespace ricanontherun