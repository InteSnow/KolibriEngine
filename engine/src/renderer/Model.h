#pragma once
#include "defines.h"
#include "renderer/Texture.h"
#include "utils/sml.hpp"
#include "core/Component.h"

struct Vertex {
  vec3 pos;
  vec3 normal;
  vec3 color;
  vec2 texCoords;
};

enum ShadingType {
  SHADE_UNLIT = 0,
  SHADE_LIT = 1
};

class Model : public KEComponent {
  Vertex* vertices;
  uint32 size;
  ShadingType shading;

  bool hasDT;
  Texture diffTex;

  vec3 color;

  void onDraw(void) override;
  void onUnregister(void) override;
public:
  static Model create(Vertex* vertices, uint32 size, const char* diffuse = NULL, ShadingType shading = SHADE_UNLIT);
  static void destroy(Model& model);
  void draw(void);
  void setColor(vec3 color);
  vec3 getColor(void);

};