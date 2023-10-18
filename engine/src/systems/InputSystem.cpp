#include "InputSystem.h"
#include "core/Events.h"
#include <cstring>

KeyboardState InputSystem::pKeyboard;
KeyboardState InputSystem::cKeyboard;
MouseState InputSystem::pMouse;
MouseState InputSystem::cMouse;

bool InputSystem::init() {
  return true;
}

void InputSystem::shutdown() {

}

void InputSystem::update() {
  memcpy(&pKeyboard, &cKeyboard, sizeof(KeyboardState));
  memcpy(&pMouse, &cMouse, sizeof(MouseState));
}

void InputSystem::processKey(KeyCode key, bool down) {
  cKeyboard.keys[key] = down;
  if (wasKeyDown(key) != down) {
    keOnKey.fire(key, down);
  }
}

void InputSystem::processButton(ButtonCode button, bool down) {
  cMouse.buttons[button] = down;
  if (wasButtonDown(button) != down) {
    keOnMouseButton.fire(button, down);
  }
}
void InputSystem::processMouse(int16 x, int16 y) {
  cMouse.x = x;
  cMouse.y = y;

  keOnMouseMove.fire(x, y);
}

void InputSystem::processWheel(int8 delta) {
  keOnMouseWheel.fire(delta);
}

bool InputSystem::wasKeyUp(KeyCode key) {
  return !pKeyboard.keys[key];
}

bool InputSystem::wasKeyDown(KeyCode key) {
  return pKeyboard.keys[key];
}

bool InputSystem::isKeyUp(KeyCode key) {
  return !cKeyboard.keys[key];
}

bool InputSystem::isKeyDown(KeyCode key) {
  return cKeyboard.keys[key];
}

bool InputSystem::wasButtonUp(ButtonCode button) {
  return !pMouse.buttons[button];
}

bool InputSystem::wasButtonDown(ButtonCode button) {
  return pMouse.buttons[button];
}

bool InputSystem::isButtonUp(ButtonCode button) {
  return !cMouse.buttons[button];
}

bool InputSystem::isButtonDown(ButtonCode button) {
  return cMouse.buttons[button];
}

void InputSystem::getMousePos(int16& x, int16& y) {
  x = cMouse.x;
  y = cMouse.y;
}

void InputSystem::getPrevMousePos(int16& x, int16& y) {
  x = pMouse.x;
  y = pMouse.y;
}