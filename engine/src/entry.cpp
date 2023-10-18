#include "GameManager.h"
#include "core/logger.h"
#include "core/EngineLoop.h"

static EngineLoop engineLoop;

int main() {
  if (!engineLoop.init()) {
    KE_ERROR("Failed to start engine loop.");
    return 1;
  }

  while (!shouldExit) {
    if (!engineLoop.tick()) {
      KE_ERROR("Engine Runtime error.");
      break;
    }
  }

  engineLoop.exit();
  
  return 0;
}