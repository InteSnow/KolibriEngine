#pragma once
#include "defines.h"
#include "Camera.h"

class Renderer {
  static uint16 frameWidth;
  static uint16 frameHeight;

  friend void Camera::updateAll(float32);
public:
  static Camera* camera;
  static bool init(uint16 width, uint16 height);

  static void shutdown(void);

  static void startFrame(void);
  static void endFrame(void);

  static void startGUI(void);
  static void endGUI(void);
  static void startText(void);

  static void onResize(uint16 width, uint16 height);
  static void onKey(uint8 key, bool down);

  static vec2 getFrameSize(void);
};