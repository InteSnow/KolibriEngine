#pragma once
#include "defines.h"

enum ShaderType {
  SHADER_VERTEX,
  SHADER_FRAGMENT
};

class Shader {
  uint32 program;

public:
  Shader();
  Shader(const char* vertexPath, const char* fragmentPath);
  void use(void);
  void destroy(void);

  void setInt(const char* name, int x);
  void setMat4(const char* name, const float* data);
};
