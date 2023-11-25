#pragma once
#include "defines.h"

struct Texture {
  static Texture create(const char* path);

  static void destroy(const Texture& tex);

  void bind(void);

private:
  uint32 handle;
};