#pragma once
#include "defines.h"
#include "gui/Rect.h"

class SceneObject;
class GUIObject;

class SceneComponent {
  virtual void onRegister(void) {};
  virtual void onUnregister(void) {};
  virtual void Update(void) {};

  virtual void onRenderBegin(void) {};
  virtual void onDraw(void) {};
  virtual void onRenderEnd(void) {};

  friend SceneObject;
public:
  virtual ~SceneComponent(void) = default;
};

class GUIComponent {
  virtual void onRegister(void) {};
  virtual void onUnregister(void) {};
  virtual void Update(void) {};

  virtual void onShapeDraw(void) {};
  virtual void onTextDraw(void) {};

  friend GUIObject;
public:
  virtual ~GUIComponent(void) = default;

  Rect* container;
};