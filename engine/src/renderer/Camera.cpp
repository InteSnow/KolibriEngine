#include "renderer/Camera.h"
#include "systems/InputSystem.h"
#include "core/logger.h"
#include <unordered_set>

#define SPEED 2.0f
#define SENSITIVITY 30.0f

enum DirectionFlagBits {
  DIR_FORWARD = 0x01,
  DIR_BACKWARD = 0x02,
  DIR_RIGHT = 0x04,
  DIR_LEFT = 0x08
};

static std::unordered_set<Camera*> cameras;

Camera* Camera::create(float32 fov, vec3 pos, vec3 front, vec3 up) {
  Camera* cam = new Camera;

  cam->fov = fov;
  cam->pos = pos;
  cam->front = normalize(front);
  cam->up = normalize(up);
  cam->yaw = 180.0f;
  cam->pitch = 0.0f;
  cam->right = normalize(cross(cam->front, cam->up));
  cameras.insert(cam);
  return cam;
}

void Camera::destroy(Camera* cam) {
  cameras.erase(cam);
  delete cam;
}

void Camera::updateAll(float32 deltaTime) {
  //KE_INFO("UPS = %f", 1/deltaTime);
  for (auto* cam : cameras) {
    if (!cam->inputEnabled) continue;

    DirectionFlags flags = 0;
    if (InputSystem::isKeyDown(KEY_W)) flags |= DIR_FORWARD;
    if (InputSystem::isKeyDown(KEY_S)) flags |= DIR_BACKWARD;
    if (InputSystem::isKeyDown(KEY_D)) flags |= DIR_RIGHT;
    if (InputSystem::isKeyDown(KEY_A)) flags |= DIR_LEFT;
    if (flags) cam->move(flags, deltaTime);

    int16 dx, dy;
    InputSystem::getMousePos(dx, dy);
    if (!dx && !dy) continue;

    float distance = deltaTime*SENSITIVITY;
    cam->yaw -= dx * distance;
    cam->pitch -= dy * distance;
  
    cam->pitch = clamp(cam->pitch, -89.0f, 89.0f);
  
    if (cam->yaw >= 360.0f) cam->yaw -= 360.0f;
    else if (cam->yaw <= -360.0f) cam->yaw += 360.0f;
    vec3 newDirection = normalize(vec3(
        sinf(radians(cam->yaw))*cosf(radians(cam->pitch)),
        sinf(radians(cam->pitch)),
        cosf(radians(cam->yaw))*cosf(radians(cam->pitch))
    ));
    
    cam->front = newDirection;
    cam->right = normalize(cross(cam->front, cam->up));
  }
}

mat4 Camera::getView() {
  return lookAt(this->pos, this->pos+this->front, this->up);
}

mat4 Camera::getProjection(uint16 width, uint16 height) {
  return perspective(this->fov, (float32)width/height, 0.1f, 100.0f);
}

void Camera::move(DirectionFlags dir, float32 deltaTime) {
  vec3 vDir = vec3(0.0f);
  if (dir & DIR_FORWARD) vDir = vDir + this->front;
  if (dir & DIR_BACKWARD) vDir = vDir - this->front;
  if (dir & DIR_RIGHT) vDir = vDir + this->right;
  if (dir & DIR_LEFT) vDir = vDir - this->right;
  if (vDir.x == 0 && vDir.y == 0 &&  vDir.z == 0) return;
  vDir = normalize(vDir);
  this->pos = this->pos + vDir * deltaTime * SPEED;
}

void Camera::switchInput() {
  this->inputEnabled = !this->inputEnabled;
}

