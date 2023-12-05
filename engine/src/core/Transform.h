#pragma once
#include "defines.h"
#include "utils/sml.hpp"
#include "core/Component.h"

class Transform {
  mat4 getModel(void);
public:
  Transform(void);
  
  vec3 position;
  vec3 size;
  vec3 rotation;

  void move(vec3 translation);
  void scale(vec3 factor);
  void rotate(vec3 rotation);

  void push(void);
  void pop(void);
};