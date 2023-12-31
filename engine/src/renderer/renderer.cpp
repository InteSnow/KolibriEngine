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

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_TEXTURE_2D);

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  
  camera = Camera::create(45.0f, vec3(0, 0, 3.0f));
  projection = camera->getProjection(frameWidth, frameHeight);

  return 1;
}

void Renderer::shutdown() {
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

void Renderer::startInterface() {
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  mat4 m = ortho(0.f, (float)frameWidth, 0.f, (float)frameHeight, 0.1f, 100.0f);
  glLoadMatrixf(m.data());
}

void Renderer::endInterface() {
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
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