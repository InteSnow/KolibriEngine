#include "Light.h"
#include "core/logger.h"
#include <unordered_set>
#include GL_HEADER

#define MAX_LIGHTS 8

static std::unordered_set<uint32> lightSlots({0, 1, 2, 3, 4, 5, 6, 7});

void Light::enable() {
  if (this->enabled) return;
  if (lightSlots.size() == 0) {
    KE_WARN("Not enough free lighting slots");
    return;
  }
  this->slot = *lightSlots.begin();
  lightSlots.erase(slot);

  glEnable(GL_LIGHT0 + this->slot);

  vec4 col = vec4(this->color, 1.0f);
  glLightfv(GL_LIGHT0+this->slot, GL_DIFFUSE, &col.x);
  
  switch (this->type) {
  case LIGHT_DIRECT:
    glLightf(GL_LIGHT0+this->slot, GL_SPOT_CUTOFF, 180.0f);
  break;
  case LIGHT_POINT:
    glLightf(GL_LIGHT0+this->slot, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0+this->slot, GL_LINEAR_ATTENUATION, this->point.falloff/2);
    glLightf(GL_LIGHT0+this->slot, GL_QUADRATIC_ATTENUATION, this->point.falloff);
    glLightf(GL_LIGHT0+this->slot, GL_SPOT_CUTOFF, 180.0f);
  break;
  case LIGHT_SPOT:
    glLightf(GL_LIGHT0+this->slot, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0+this->slot, GL_LINEAR_ATTENUATION, this->spot.falloff/2);
    glLightf(GL_LIGHT0+this->slot, GL_QUADRATIC_ATTENUATION, this->spot.falloff);
    glLightf(GL_LIGHT0+this->slot, GL_SPOT_CUTOFF, this->spot.angle/2);
    glLightf(GL_LIGHT0+this->slot, GL_SPOT_EXPONENT, this->spot.exp);
  break;
  }
  this->enabled = 1;
}

void Light::update() {
  if (this->slot < 0) return;
  vec4 dir, pos;

  switch (this->type) {
  case LIGHT_DIRECT:
    dir = vec4(-this->direct.dir, 0);
    glLightfv(GL_LIGHT0+this->slot, GL_POSITION, &dir.x);
  break;
  case LIGHT_POINT:
    pos = vec4(this->point.pos, 1);
    glLightfv(GL_LIGHT0+this->slot, GL_POSITION, &pos.x);
  break;
  case LIGHT_SPOT:
    pos = vec4(this->spot.pos, 1);
    glLightfv(GL_LIGHT0+this->slot, GL_POSITION, &pos.x);
    glLightfv(GL_LIGHT0+this->slot, GL_SPOT_DIRECTION, &this->spot.dir.x);
  break;
  }
}

void Light::disable() {
  if (this->slot < 0) return;
  glDisable(GL_LIGHT0 + this->slot);
  lightSlots.insert(this->slot);
  this->slot = -1;
  this->enabled = 0;
}


void Light::setColor(vec3 color) {
  this->color = color;
  if (this->enabled) glLightfv(GL_LIGHT0+this->slot, GL_DIFFUSE, &this->color.x);
}

void Light::setDir(vec3 dir) {
  this->direct.dir = dir;
  this->spot.dir = dir;
}

void Light::setPos(vec3 pos) {
  this->point.pos = pos;
  this->spot.pos = pos;
}

void Light::setFalloff(float falloff) {
  this->point.falloff = falloff;
  this->spot.falloff = falloff;
  if (this->enabled) {
    glLightf(GL_LIGHT0+this->slot, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0+this->slot, GL_LINEAR_ATTENUATION, this->point.falloff/2);
    glLightf(GL_LIGHT0+this->slot, GL_QUADRATIC_ATTENUATION, this->point.falloff);
  }
}

void Light::setAngle(float angle) {
  this->spot.angle = angle;
  if (this->enabled) glLightf(GL_LIGHT0+this->slot, GL_SPOT_CUTOFF, this->spot.angle/2);
}

void Light::setExponent(float exp) {
  this->spot.exp = exp;
  if (this->enabled) glLightf(GL_LIGHT0+this->slot, GL_SPOT_EXPONENT, this->spot.exp);
}


vec3 Light::getColor() {
  return this->color;
}

vec3 Light::getDir() {
  return this->direct.dir;
}

vec3 Light::getPos() {
  return this->point.pos;
}

float Light::getFalloff() {
  return this->point.falloff;
}

float Light::getAngle() {
  return this->spot.angle;
}

float Light::getExponent() {
  return this->spot.exp;
}
