#pragma once
#include "defines.h"
#include "core/Component.h"
#include "Mesh.h"
#include <vector>

class Model : public KEComponent {
  std::vector<Mesh> meshes;
  ShadingType shading;

  void onDraw(void) override;
  void onUnregister(void) override;

public:

  static Model create(const char* path, ShadingType shading = SHADE_UNLIT);
  static void destroy(Model& model);

};