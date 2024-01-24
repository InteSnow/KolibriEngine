#pragma once
#include "defines.h"
#include "core/Component.h"
#include "renderer/Texture.h"
#include "physics/Collider.h"
#include "utils/sml.hpp"
#include <vector>
#include <string>

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

class Model;

class Mesh : public SceneComponent {
  std::vector<Vertex> vertices;

  bool hasDT;
  Texture diffTex;
  ShadingType shading;

  void onDraw(void) override;
  void onUnregister(void) override;

  friend Model;
public:
  static Mesh create(std::vector<Vertex>&& vertices, ShadingType shading, std::string diffuse = "");
  static void destroy(Mesh& mesh);
  BoundingBox getBoundingBox(void) const;
};