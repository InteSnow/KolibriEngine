#pragma once
#include "defines.h"
#include "utils/sml.hpp"

typedef uint32 DirectionFlags;

class Camera {
  vec3 pos;

  vec3 up;
  vec3 front;
  vec3 right;

  float fov;
  float yaw;
  float pitch;

  bool inputEnabled = true;

  Camera(void) = default;

  void move(DirectionFlags dir, float32 deltaTime);
public:

  static Camera* create(float32 fov = 90.0f, vec3 pos = vec3(0.0f),
   vec3 front = vec3(0.0f, 0.0f, -1.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f));


  static void destroy(Camera* cam);

  static void updateAll(float32 deltaTime);

  mat4 getView(void);
  mat4 getProjection(uint16 width, uint16 height);

  void switchInput(void);
};