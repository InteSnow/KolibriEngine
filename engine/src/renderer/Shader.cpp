#include "renderer/Shader.h"
#include "core/File.h"
#include "core/logger.h"
#include GL_HEADER

static uint32 compileShader(ShaderType type, const char* path) {
  File f = File::open(path, FILE_READ);
  std::string shaderStr;
  f.readAll(shaderStr);
  File::close(f);
  const char* shaderSource = shaderStr.c_str();

  uint32 shader = glCreateShader(type == SHADER_VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);

  int32  success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    KE_ERROR("SHADER: compilation of %s failed (%s)", path, infoLog);
  }

  return shader;
}

Shader::Shader() {}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  uint32 vertexShader = compileShader(SHADER_VERTEX, vertexPath);
  uint32 fragmentShader = compileShader(SHADER_FRAGMENT, fragmentPath);

  this->program = glCreateProgram();
  glAttachShader(this->program, vertexShader);
  glBindAttribLocation(this->program, 0, "vPos");
  glBindAttribLocation(this->program, 1, "vCol");
  glBindAttribLocation(this->program, 2, "vTexCoords");
  glAttachShader(this->program, fragmentShader);
  glLinkProgram(this->program);

  int  success;
  char infoLog[512];
  glGetProgramiv(this->program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(this->program, 512, nullptr, infoLog);
    KE_ERROR("Program linking failed: %s", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() {
  glUseProgram(this->program);
}

void Shader::destroy() {
  glDeleteProgram(this->program);
}

void Shader::setInt(const char* name, int x) {
  glUniform1i(glGetUniformLocation(this->program, name), x);
}

void Shader::setMat4(const char* name, const float* data) {
  glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, data);
}

