#pragma once
#include "defines.h"
#include "utils/sml.hpp"
#include "renderer/Texture.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <stdarg.h>
#include <string>

std::string sformat(const char* format, va_list vl);

struct Character {
  Texture tex;
  vec2 bearing;
  vec2 size;
  uint32 advance;
};

struct Font {
  std::unordered_map<uint32, Character> characters;
  uint32 height;
  uint32 median;
  const char* familyName;
};

class Fonts {
  static FT_Library ft;
  static Font* activeFont;
public:
  static bool init(void);
  static void shutdown(void);

  static bool load(const char* path, uint32 height, uint32* glyphs, uint32 glyphCount);

  static bool select(const char* familyName, uint32 height);

  static uint32 getWidth(uint32 height, const char* format, ...);
  static void print(uint32 x, uint32 y, uint32 height, vec3 color, const char* format, ...);

  static uint32 getWidth(uint32 height, const char* format, va_list vl);
  static void print(uint32 x, uint32 y, uint32 height, vec3 color, const char* format, va_list vl);

  static const Font* getActive(void);
};