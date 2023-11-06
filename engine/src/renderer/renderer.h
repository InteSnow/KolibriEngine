#pragma once
#include "defines.h"
#include "Shader.h"
#include "Camera.h"

class Renderer {
  static uint16 frameWidth;
  static uint16 frameHeight;

  static Shader shader;

  static Camera* camera;

public:
  static bool init(uint16 width, uint16 height);

  static void shutdown(void);

  static void startFrame(float deltaTime);

  static void endFrame(void);

  static void onResize(uint16 width, uint16 height);
  static void onKey(uint8 key, bool down);
};