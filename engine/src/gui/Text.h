#pragma once
#include "defines.h"
#include "Rect.h"
#include "utils/sml.hpp"
#include <string>
#include <cstdarg>

class Text {
  std::wstring text;

public:
  Rect* container;
  
  uint32 height;
  vec3 color;
  bool center;

  const char* familyName;
  uint32 fontHeight;

  Text(Rect* container = NULL, const char* familyName = NULL, uint32 fontHeight = 0, uint32 height = 0, vec3 color = vec3(1.0f));

  void setText(const wchar_t* format, ...);
  void setText(const wchar_t* format, va_list vl);
  void onDraw(void);
};