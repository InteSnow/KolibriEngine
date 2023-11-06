#pragma once
#include "defines.h"
#include <fstream>
#include <string>

enum FileAccessBits {
  FILE_READ = 1,
  FILE_WRITE = 2,
  FILE_BINARY = 4,
  FILE_APPEND = 8,
  FILE_TRUNC = 16
};

typedef uint32 FileAccess;

class File {
  std::fstream* handle;
  FileAccess access;
  uint64 size;

  public:
  
  static File open(const char* path, FileAccess mode = FILE_READ | FILE_WRITE);
  static void close(File file);

  void readAll(std::string& buffer);

};

