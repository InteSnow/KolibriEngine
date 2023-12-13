#pragma once
#include "defines.h"
#include <string>

struct Texture {
  static Texture create(std::string path);

  static void destroy(const Texture& tex);

  void bind(int32 slot);

  void unbind(void);

private:
  uint32 handle;
  int32 slot;
};