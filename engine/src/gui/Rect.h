#pragma once
#include "defines.h"
#include "utils/sml.hpp"
#include "core/Component.h"

class GUI;

class Rect {
  vec2 position;
  vec2 dimensions;
  float radius;
  vec3 color;

  friend GUI;

public:
  Rect(uint32 x = 0, uint32 y = 0, uint32 width = 0, uint32 height = 0, uint32 radius = 0, vec3 color = vec3(1.0f));

  vec2 getPosition(void) const;
  vec2 getDimensions(void) const;
  float getRadius(void) const;
  vec3 getColor(void) const;

  void setPosition(uint32 x, uint32 y);
  void setDimensions(uint32 width, uint32 height);
  void setRadius(uint32 radius);
  void setColor(vec3 color);

  bool isHit(int16 x, int16 y) const;

  void onShapeDraw(void);
  void onTextDraw(void);
};