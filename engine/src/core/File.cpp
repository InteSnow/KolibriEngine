#include "core/File.h"
#include "core/logger.h"
#include <fstream>
#include <sstream>

File File::open(const char* path, FileAccess mode) {
  File file = {};
  file.access = mode;
  std::fstream::open_mode openmode = (mode & FILE_READ ? std::fstream::in : 0) 
  | (mode & FILE_WRITE ? std::fstream::out : 0) | (mode & FILE_BINARY ? std::fstream::binary : 0)
  | (mode & FILE_APPEND ? std::fstream::app : 0) | (mode & FILE_TRUNC ? std::fstream::trunc : 0)
  | std::fstream::ate;

  file.handle = new std::fstream(path, (std::fstream::openmode)openmode);
  if (!file.handle->is_open()) {
    KE_ERROR("FILE: failed to open %s", path);
    return file;
  }

  file.size = file.handle->tellg();
  file.handle->seekg(std::fstream::beg);

  return file;
}

void File::close(File file) {
  file.handle->close();
  delete file.handle;
}

void File::readAll(std::string& buffer) {
  if (this->access & FILE_READ == 0 || !this->handle->is_open()) return;
  std::ostringstream content;
  content << this->handle->rdbuf();

  buffer = content.str();
}