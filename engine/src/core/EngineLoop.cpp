#include "EngineLoop.h"
#include "core/logger.h"
#include "core/Events.h"
#include "core/Object.h"
#include "platform/platform.h"
#include "systems/InputSystem.h"
#include "systems/Fonts.h"
#include "systems/GUI.h"
#include "renderer/renderer.h"

bool shouldExit = false;

static void exitLoop(void);
static void keyCallback(uint8 key, bool down);

static TimeInfo _Time = {};

const TimeInfo* Time = &_Time;
float fps;
bool EngineLoop::init() {
  InputSystem::init();

  if (!platformInit("Kolibri Engine", 100, 100, 800, 600)) {
    return 0;
  }
  Fonts::init();
  GUI::init();
  InputSystem::setCapture(true);
  Renderer::init(800, 600);
  
  keOnQuit.subscribe(exitLoop);
  keOnKey.subscribe(keyCallback);

  this->gameManager.init();
  return 1;
}

bool EngineLoop::tick() { 
  static uint64 start = getTime();
  static uint64 end;

  pollEvents();
  InputSystem::update();
  Camera::updateAll(_Time.deltaTime);
  for (SceneObject* obj : SceneObject::objects) {
    obj->Update();
  }
  for (GUIObject* obj : GUIObject::objects) {
    obj->Update();
  }
  this->gameManager.update();

  end = getTime();
  _Time.deltaTime = (end - start)/1000000.0f;
  _Time.time += _Time.deltaTime;
  fps = 1000000.0f/(end - start);
  //KE_WARN("FPS = %f", 1000000.0f/(end - start));

  Renderer::startFrame();
  for (SceneObject* obj : SceneObject::objects) {
    obj->onRenderBegin();
  }
  for (SceneObject* obj : SceneObject::objects) {
    obj->onDraw();
  }
  for (SceneObject* obj : SceneObject::objects) {
    obj->onRenderEnd();
  }
  Renderer::startGUI();
  for (GUIObject* obj : GUIObject::objects) {
    obj->onShapeDraw();
  }
  Renderer::startText();
  for (GUIObject* obj : GUIObject::objects) {
    obj->onTextDraw();
  }
  this->gameManager.render();
  Renderer::endGUI();
  Renderer::endFrame(); 

  platformPresent();
  start = end;

  return 1;
}

void EngineLoop::exit() {

  this->gameManager.shutdown();

  for (SceneObject* obj : SceneObject::objects) {
    SceneObject::destroyNoErase(obj);
  }
  SceneObject::objects.clear();

  Renderer::shutdown();

  GUI::shutdown();
  Fonts::shutdown();

  platformShutdown();

  InputSystem::shutdown();
}

void exitLoop() {
  shouldExit = true;
}

void keyCallback(uint8 key, bool down) {
  if (key == KEY_F11 && down) {
    platformToggleFullscreen();
  }
}