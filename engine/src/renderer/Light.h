#pragma once
#include "defines.h"
#include "utils/sml.hpp"
#include "core/Component.h"

enum LightType {
  LIGHT_DIRECT = 0,
  LIGHT_POINT = 1,
  LIGHT_SPOT = 2
};

struct DirectLight {
  vec3 dir;
};

struct PointLight {
  float falloff;
};

struct SpotLight {
  vec3 dir;
  float falloff;
  float angle;
  float exp;
};

class Light : public SceneComponent {
  uint32 slot;
  bool enabled = 0;

  DirectLight direct;
  PointLight point;
  SpotLight spot;

  void onRegister(void) override;
  void onRenderBegin(void) override;
  void onUnregister(void) override;
public:

  void enable(void);
  void update(void);  
  void disable(void);
  
  vec3 color;

  LightType type;

  void setColor(vec3 color);
  void setDir(vec3 dir);
  void setFalloff(float falloff);
  void setAngle(float angle);
  void setExponent(float exp);

  vec3 getColor(void);
  vec3 getDir(void);
  float getFalloff(void);
  float getAngle(void);
  float getExponent(void);

};
