#pragma once
#include "defines.h"
#include "utils/sml.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Font;

class Fonts {
  static FT_Library ft;
  static Font* activeFont;
public:
  static bool init(void);
  static void shutdown(void);

  static bool load(const char* path, uint32 height, uint32* glyphs, uint32 glyphCount);

  static bool select(const char* familyName, uint32 height);

  static void print(uint32 x, uint32 y, uint32 height, vec3 color, const char* format, ...);
};