#include "EngineLoop.h"
#include "platform/platform.h"
#include "systems/InputSystem.h"
#include "core/Events.h"
#include "renderer/renderer.h"
#include <chrono>

bool shouldExit = false;

static void exitLoop(void);

bool EngineLoop::init() {
  InputSystem::init();

  if (!platformInit("Kolibri Engine", 100, 100, 400, 400)) {
    return 0;
  }

  Renderer::init();

  keOnQuit.subscribe(exitLoop);

  this->gameManager.init();

  return 1;
}

bool EngineLoop::tick() { 
  static uint64 start = getTime();
  uint64 end = getTime();

  InputSystem::update();
  pollEvents();
  if (end - start < 1000000.0f/144) return 1;

  this->gameManager.update();

  Renderer::startFrame((end - start)/1000000.0f);
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