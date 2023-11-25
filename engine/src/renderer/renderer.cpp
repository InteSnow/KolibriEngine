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

  keOnResize.subscribe(Renderer::onResize);
  keOnKey.subscribe(Renderer::onKey);

  glClearColor(0.1f, 0.1f, 0.1f, 1);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

#ifdef KE_PLATFORM_WIN32
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
  glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
  
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PRIMARY_COLOR);
  glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_ALPHA);
#endif

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  glEnable(GL_TEXTURE_2D);

  camera = Camera::create(45.0f, vec3(0, 0, 3.0f));
  projection = camera->getProjection(frameWidth, frameHeight);
}

void Renderer::shutdown() {
  Camera::destroy(Renderer::camera);
}

void Renderer::startFrame(void) {
  Camera::updateAll(Time->deltaTime);
  
	mat4 view = Renderer::camera->getView();
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection.data());
  glMultMatrixf(view.data());
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
	glFlush();
}

void Renderer::onResize(uint16 width, uint16 height) {
  frameWidth = width;
  frameHeight = height;
  projection = camera->getProjection(frameWidth, frameHeight);
  glViewport(0, 0, frameWidth, frameHeight);
}

void Renderer::onKey(uint8 key, bool down) {
  if (key == KEY_ESCAPE && down) {
    camera->switchInput();
    InputSystem::setCapture(!InputSystem::getCapture());
  }
}