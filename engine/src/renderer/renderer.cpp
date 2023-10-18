#include "renderer.h"
#include "core/logger.h"
#include "utils/sml.hpp"
#ifdef KE_PLATFORM_KOS32
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#elif defined(KE_PLATFORM_WIN32)
#define GLEW_STATIC
#include <GL/glew.h>
#endif

static float32 vertices[] = {
  //  X      Y      Z     R     G     B
  -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
   0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
   0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f
};

static uint32 VAO;
static uint32 VBO;
static uint32 vertexShader;
static uint32 fragmentShader;
static uint32 shaderProgram;

static mat4 model;

static const char* vertexSource = 
"attribute vec3 vPos;\n\
attribute vec3 vCol;\n\
varying vec3 fCol;\n\
uniform mat4 model;\n\
void main()\n\
{\n\
   gl_Position = model*vec4(vPos, 1.0);\n\
   fCol = vCol;\n\
}\0";
static const char* fragmentSource = 
"varying vec3 fCol;\n\
void main() {\n"
  "gl_FragColor = vec4(fCol, 1.0);\n"
"}\0";

bool Renderer::init() {
  glClearColor(0.1f, 0.1f, 0.1f, 1);
  KE_INFO("%s", glGetString(GL_VERSION));
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);
  
  int  success1;
  char infoLog1[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success1);
  
  if (!success1) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog1);
    KE_ERROR("Vertex shader compilation failed: %s", infoLog1);
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentShader);
 
  int  success2;
  char infoLog2[512];
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);
  if (!success2) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog2);
    KE_ERROR("Fragment shader compilation failed: %s", infoLog2);
  }

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glBindAttribLocation(shaderProgram, 0, "vPos");
  glBindAttribLocation(shaderProgram, 1, "vCol");
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int  success3;
  char infoLog3[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);
  if (!success3) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog3);
    KE_ERROR("Program linking failed: %s", infoLog3);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float32), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float32), (void*)(3*sizeof(float32)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);

  model = mat4(1.0f);
}

void Renderer::shutdown() {

}

void Renderer::startFrame(float deltaTime) {
  model = rotate(model, 30.0f*deltaTime, vec3(0, 0, 1.0f));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  int modelLoc = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::endFrame() {
	glFlush();
}