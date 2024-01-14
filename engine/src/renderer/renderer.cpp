#include "renderer.h"
#include "core/logger.h"
#include "utils/sml.hpp"
#include "core/Events.h"
#include "renderer/Camera.h"
#include "systems/InputSystem.h"
#include GL_HEADER

static mat4 view;
static mat4 projection;

uint16 Renderer::frameWidth;
uint16 Renderer::frameHeight;
Camera* Renderer::camera;

#ifdef KE_PLATFORM_WIN32
uint32 guiShader;
#include "core/File.h"
#endif

bool Renderer::init(uint16 width, uint16 height) {
  frameWidth = width;
  frameHeight = height;

  keOnKey.subscribe(Renderer::onKey);
  keOnResize.subscribe(Renderer::onResize);
  glClearColor(0.1f, 0.1f, 0.1f, 1);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

#ifdef KE_PLATFORM_WIN32
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  
  camera = Camera::create(45.0f, vec3(0, 0, 3.0f));
  projection = camera->getProjection(frameWidth, frameHeight);

#ifdef KE_PLATFORM_WIN32
  uint32 vertex = glCreateShader(GL_VERTEX_SHADER);
  File vertexFile = File::open("resources/shaders/gui.vert", FILE_READ);
  std::string vertexString;
  vertexFile.readAll(vertexString);
  File::close(vertexFile);
  const char* vertexSource = vertexString.c_str();
  glShaderSource(vertex, 1, &vertexSource, NULL);
  glCompileShader(vertex);

  uint32 fragment = glCreateShader(GL_FRAGMENT_SHADER);
  File fragmentFile = File::open("resources/shaders/gui.frag", FILE_READ);
  std::string fragmentString;
  fragmentFile.readAll(fragmentString);
  const char* fragmentSource = fragmentString.c_str();
  glShaderSource(fragment, 1, &fragmentSource, NULL);
  glCompileShader(fragment);

  guiShader = glCreateProgram();
  glAttachShader(guiShader, vertex);
  glAttachShader(guiShader, fragment);
  glLinkProgram(guiShader);

  int32 success;
  char infoLog[512];
  glGetProgramiv(guiShader, GL_LINK_STATUS, &success);
  if(!success) {
    glGetProgramInfoLog(guiShader, 512, NULL, infoLog);
    KE_INFO("Shader program linking failed: %s", infoLog);
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
#endif

  return 1;
}

void Renderer::shutdown() {
#ifdef KE_PLATFORM_WIN32
  glDeleteProgram(guiShader);
#endif
  Camera::destroy(Renderer::camera);
}

void Renderer::startFrame(void) {
	mat4 view = Renderer::camera->getView();
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(view.data()); 

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection.data());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
	glFlush();
}

void Renderer::startGUI() {
#ifdef KE_PLATFORM_WIN32
  glUseProgram(guiShader);
  //glSetFragShader(GL_FRAG_QUADS);
#else
  glSetFragShader(GL_FRAG_QUADS);
#endif

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  mat4 m = ortho(0.f, (float)frameWidth, 0.f, (float)frameHeight);
  glLoadMatrixf(m.data());
}

void Renderer::endGUI() {
#ifdef KE_PLATFORM_KOS32
  glSetFragShader(GL_FRAG_DEFAULT);
#endif
  //glSetFragShader(GL_FRAG_DEFAULT);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
}

void Renderer::startText() {
#ifdef KE_PLATFORM_WIN32
  //glSetFragShader(GL_FRAG_TEXT);
  glUseProgram(0);
#else
  glSetFragShader(GL_FRAG_TEXT);
#endif
}

void Renderer::onResize(uint16 width, uint16 height) {
  frameWidth = width;
  frameHeight = height;
  projection = camera->getProjection(frameWidth, frameHeight);
  glViewport(0, 0, width, height);
}

void Renderer::onKey(uint8 key, bool down) {
  if (key == KEY_ESCAPE && down) {
    camera->switchInput();
    InputSystem::setCapture(!InputSystem::getCapture());
  }
}

vec2 Renderer::getFrameSize(void) {
  return vec2(frameWidth, frameHeight);
}