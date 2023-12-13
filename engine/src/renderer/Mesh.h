#pragma once
#include "defines.h"
#include "renderer/Texture.h"
#include "utils/sml.hpp"
#include "core/Component.h"
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

class Mesh {
  std::vector<Vertex> vertices;

  bool hasDT;
  Texture diffTex;

  void draw(void);

  friend Model;
public:
  static Mesh create(std::vector<Vertex>&& vertices, std::string diffuse = "");
  static void destroy(Mesh& mesh);
};