#pragma once
#include "defines.h"

class Renderer {
public:
  static bool init(void);

  static void shutdown(void);

  static void startFrame(float deltaTime);

  static void endFrame(void);
};