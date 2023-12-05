#pragma once
#include "defines.h"

class GameObject;

class KEComponent {
  virtual void onRegister(void);
  virtual void onUnregister(void);

  virtual void onRenderBegin(void);
  virtual void onDraw(void);
  virtual void onRenderEnd(void);

  friend GameObject;
public:
  virtual ~KEComponent(void) = default;
};