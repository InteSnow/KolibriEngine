#pragma once
#include "defines.h"
#include "renderer/Texture.h"
#include "utils/sml.hpp"

struct Vertex {
  vec3 pos;
  vec4 color;
  vec2 texCoords;
};

void draw(Vertex* vertices, uint32 size);