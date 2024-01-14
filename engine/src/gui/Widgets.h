#pragma once
#include "defines.h"
#include "core/Component.h"
#include "utils/sml.hpp"
#include "systems/InputSystem.h"

class GUI;

class Button;

typedef void (*pfnButtonHoverCallback)(Button* handle);
typedef void (*pfnButtonClickCallback)(Button* handle, ButtonCode mbutton);

class Button : public GUIComponent {
  friend GUI;

protected:
  bool hovered;
  bool pressed[_BUTTON_CODE_MAX_ENUM];

public:
  Button();

  bool isHovered() const;
  bool isPressed(ButtonCode button) const;

  virtual void onHover(void);
  virtual void onUnhover(void);
  virtual void onPress(ButtonCode button);
  virtual void onRelease(ButtonCode button);

  void onRegister(void) override final;
  void onUnregister(void) override final;
};

#include "core/Object.h"