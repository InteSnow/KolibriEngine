#pragma once
#include "defines.h"
#include "Rect.h"
#include "utils/sml.hpp"
#include <string>

class Text {
  std::string text;

public:
  Rect* container;
  
  uint32 height;
  vec3 color;

  const char* familyName;
  uint32 fontHeight;

  Text(Rect* container = NULL, const char* familyName = NULL, uint32 fontHeight = 0, uint32 height = 0, vec3 color = vec3(1.0f));

  void setText(const char* format, ...);
  void onDraw(void);
};