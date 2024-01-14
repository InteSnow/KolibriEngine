#include "Widgets.h"
#include "renderer/renderer.h"
#include "systems/GUI.h"

Button::Button() {
  this->hovered = 0;
  for (uint32 i = 0; i < _BUTTON_CODE_MAX_ENUM; i++) {
    this->pressed[i] = 0;
  }
}

bool Button::isHovered() const {
  return this->hovered;
}

bool Button::isPressed(ButtonCode button) const {
  return this->pressed[button];
}

void Button::onHover() {
  this->hovered = true;
}

void Button::onUnhover() {
  this->hovered = false;
}

void Button::onPress(ButtonCode button) {
  this->pressed[button] = 1;
}

void Button::onRelease(ButtonCode button) {
  this->pressed[button] = 0;
}

void Button::onRegister() {
  GUI::addButton(this);
}

void Button::onUnregister() {
  GUI::removeButton(this);
}