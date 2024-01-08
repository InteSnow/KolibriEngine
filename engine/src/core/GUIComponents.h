#pragma once
#include "defines.h"
#include "Component.h"
#include "utils/sml.hpp"
#include "systems/InputSystem.h"

class GUI;

class Button;

typedef void (*pfnButtonHoverCallback)(Button* handle);
typedef void (*pfnButtonClickCallback)(Button* handle, ButtonCode mbutton);

class Button : public GUIComponent {
  vec2 position;
  vec2 dimensions;
  float radius;

  vec3 color;

  friend GUI;
public:
  bool hovered;
  bool pressed;

  Button(uint32 x = 0, uint32 y = 0, uint32 width = 0, uint32 height = 0, uint32 radius = 0, vec3 color = vec3(1.0f));

  vec2 getPosition(void) const;
  vec2 getDimensions(void) const;
  float getRadius(void) const;
  vec3 getColor(void) const;

  void setPosition(uint32 x, uint32 y);
  void setDimensions(uint32 width, uint32 height);
  void setRadius(uint32 radius);
  void setColor(vec3 color);

  pfnButtonHoverCallback onHover;
  pfnButtonHoverCallback onUnhover;
  pfnButtonClickCallback onPress;
  pfnButtonClickCallback onRelease;

  bool isHit(int16 x, int16 y) const;

  void onRegister(void) override final;
  void onUnregister(void) override final;
  void onShapeDraw(void) override final;
};