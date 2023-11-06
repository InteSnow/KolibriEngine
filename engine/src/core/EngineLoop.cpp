#include "EngineLoop.h"
#include "platform/platform.h"
#include "systems/InputSystem.h"
#include "core/Events.h"
#include "renderer/renderer.h"
#include <chrono>

bool shouldExit = false;

static void exitLoop(void);
static float deltaTime;
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
  static uint64 ds = start;
  static uint64 end;

  pollEvents();
  InputSystem::update();
  this->gameManager.update();

  end = getTime();
  //KE_WARN("FPS = %f", 1000000.0f/(ds - start));
  Renderer::startFrame((end - start)/1000000.0f);
  Renderer::endFrame(); 

  ds = getTime();
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