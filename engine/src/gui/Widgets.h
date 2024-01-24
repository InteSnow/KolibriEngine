#pragma once
#include "defines.h"
#include "Text.h"
#include "core/Component.h"
#include "utils/sml.hpp"
#include "systems/InputSystem.h"
#include "systems/Fonts.h"

class GUI;

class Button;

class Button : public GUIComponent {
  friend GUI;

protected:
  bool hovered;
  bool pressed[_BUTTON_CODE_MAX_ENUM];

public:
  Button(void);

  bool isHovered(void) const;
  bool isPressed(ButtonCode button) const;

  virtual void onHover(void);
  virtual void onUnhover(void);
  virtual void onPress(ButtonCode button);
  virtual void onRelease(ButtonCode button);

  void onRegister(void) override final;
  void onUnregister(void) override final;
};

class Slider : public GUIComponent {
  friend GUI;

protected:
  float minVal;
  float maxVal;
  float val;

  vec3 trackColor;
  vec3 thumbColor;
  vec3 valColor;

  bool dragged;

  Button* thumb;
  Rect thumbRect;
  
  Rect track;

  const Font* font;
  Text valText;
  Rect valRect;
  uint32 valHeight;

  vec2 size;
  vec2 pos;
  float thumbX;
  
  void onRegister(void) override;
  void onUnregister(void) override;
public:
  Slider(void);


  void setRange(vec2 range);
  void setValue(float val);
  void setFont(const char* familyName, uint32 fontHeight, uint32 height, vec3 color = vec3(1.0f));
  void setColor(vec3 trackColor = vec3(1.0f), vec3 thumbColor = vec3(1.0f), vec3 valColor = vec3(1.0f));
  vec2 getRange(void) const;
  float getValue(void) const;
  const Font* getFont(void) const;
  void getColor(vec3* trackColor, vec3* thumbColor, vec3* valColor) const;

  void Update(void) override;
  void onShapeDraw(void) override;
  void onTextDraw(void) override;
};

#include "core/Object.h"