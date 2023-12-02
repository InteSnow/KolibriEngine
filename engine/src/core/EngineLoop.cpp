#include "EngineLoop.h"
#include "platform/platform.h"
#include "systems/InputSystem.h"
#include "core/Events.h"
#include "renderer/renderer.h"

bool shouldExit = false;

static void exitLoop(void);

static TimeInfo _Time = {};

const TimeInfo* Time = &_Time;

bool EngineLoop::init() {
  InputSystem::init();

  if (!platformInit("Kolibri Engine", 100, 100, 400, 400)) {
    return 0;
  }
  InputSystem::setCapture(true);
  Renderer::init(400, 400);
  
  keOnQuit.subscribe(exitLoop);
  this->gameManager.init();

  return 1;
}

bool EngineLoop::tick() { 
  static uint64 start = getTime();
  static uint64 end;

  pollEvents();
  InputSystem::update();
  Camera::updateAll(_Time.deltaTime);
  this->gameManager.update();

  end = getTime();
  _Time.deltaTime = (end - start)/1000000.0f;
  _Time.time += _Time.deltaTime;
  //KE_WARN("FPS = %f", 1000000.0f/(ds - start));

  Renderer::startFrame();
  this->gameManager.render();
  Renderer::endFrame(); 

  platformPresent();
  start = end;

  return 1;
}

void EngineLoop::exit() {
  this->gameManager.shutdown();

  Renderer::shutdown();

  platformShutdown();

  InputSystem::shutdown();
}

void exitLoop(void) {
  shouldExit = true;
}