#include "Fonts.h"
#include "core/logger.h"
#include "renderer/Texture.h"
#include "utils/sml.hpp"

#include GL_HEADER

#include <sstream>
#include <vector>

FT_Library Fonts::ft;
Font* Fonts::activeFont;

static std::string to_string(uint32 n) {
  std::ostringstream s;
  s << n ;
  return s.str();
}

std::string sformat(const char* format, va_list vl) {
  va_list vlc;
  va_copy(vlc, vl);
  uint32 len = vsnprintf(NULL, 0, format, vlc);
  va_end(vlc);

  std::vector<char> data(len+1);
  vsnprintf(data.data(), data.size(), format, vl);

  return std::string(data.data(), len);
}

static std::unordered_map<std::string, Font> fonts;

bool Fonts::init() {
  if (FT_Init_FreeType(&ft)) {
    KE_ERROR("Failed to initialize FreeType library");
    return 0;
  }
  return 1;
}

void Fonts::shutdown() {
  FT_Done_FreeType(ft);
}

bool Fonts::load(const char* path, uint32 height, uint32* glyphs, uint32 glyphCount) {
  FT_Face face;
  if (FT_New_Face(ft, path, 0, &face)) {
    KE_WARN("Failed to load font face");
    return 0;
  }

  FT_Set_Pixel_Sizes(face, 0, height);

  std::string fontName = std::string(face->family_name) + std::string(" ") + to_string(height);
  fonts[fontName].height = height;
  fonts[fontName].familyName = face->family_name;
  fonts[fontName].median = (face->ascender) >> 7;

  Character glyph = {};
  uint32 c;
  for (uint32 i = 0; i < glyphCount; i++) {
    c = glyphs[i];
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      KE_WARN("Failed to load glyph %llc", c);
      continue;
    }
    glyph.tex = Texture::create(face->glyph->bitmap.buffer, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
    glyph.bearing = vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    glyph.size = vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    glyph.advance = face->glyph->advance.x;

    fonts[fontName].characters[c] = glyph;
  }
  FT_Done_Face(face);

  Fonts::activeFont = &fonts[fontName];
  return 1;
}

bool Fonts::select(const char* familyName, uint32 height) {
  std::string fontName = std::string(familyName) + std::string(" ") + to_string(height);
  if (fonts.find(fontName) != fonts.end()) {
    Fonts::activeFont = &fonts[fontName];
    return 1;
  }
  return 0;
}

uint32 Fonts::getWidth(uint32 height, const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  uint32 width = Fonts::getWidth(height, format, vl);
  va_end(vl);
 
  return width;
}

void Fonts::print(uint32 x, uint32 y, uint32 height, vec3 color, const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  Fonts::print(x, y, height, color, format, vl);
  va_end(vl);
}

uint32 Fonts::getWidth(uint32 height, const char* format, va_list vl) {
  std::string text = sformat(format, vl);

  float scale = 1.0f;
  if (height) {
    scale = (float)height / Fonts::activeFont->height;
  }

  uint32 width = 0;
  for (uint32 i = 0; text[i]; i++) {
    Character glyph = Fonts::activeFont->characters[text[i]];

    float xpos = width + glyph.bearing.x * scale;

    width += (glyph.advance >> 6) * scale;
  }

  return width;
}


void Fonts::print(uint32 x, uint32 y, uint32 height, vec3 color, const char* format, va_list vl) {
  std::string text = sformat(format, vl);
  
  float scale = 1.0f;
  if (height) {
    scale = (float)height / Fonts::activeFont->height;
  } 

  for (uint32 i = 0; text[i]; i++) {
    Character glyph = Fonts::activeFont->characters[text[i]];

    float xpos = x + glyph.bearing.x * scale;
    float ypos = y - (glyph.size.y - glyph.bearing.y) * scale;

    float w = glyph.size.x * scale;
    float h = glyph.size.y * scale;

    glyph.tex.bind(0);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glColor3fv(&color.x); glVertex2f(xpos+0, ypos+h);
    glTexCoord2f(1.0f, 0.0f); glColor3fv(&color.x); glVertex2f(xpos+w, ypos+h);
    glTexCoord2f(1.0f, 1.0f); glColor3fv(&color.x); glVertex2f(xpos+w, ypos-0);
    glTexCoord2f(0.0f, 1.0f); glColor3fv(&color.x); glVertex2f(xpos+0, ypos-0);
    glEnd();

    glyph.tex.unbind();

    x += (glyph.advance >> 6) * scale;
  }
}

const Font* Fonts::getActive() {
  return Fonts::activeFont;
}