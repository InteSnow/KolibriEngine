#pragma once
#include "defines.h"

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

  SceneObject* parent;
};

class GUIComponent {
  virtual void onRegister(void) {};
  virtual void onUnregister(void) {};
  virtual void Update(void) {};

  friend GUIObject;
public:
  virtual ~GUIComponent(void) = default;

  GUIObject* parent;
};