#include "GUI.h"
#include "core/Events.h"

std::unordered_set<Button*> buttons;

static void guiOnMove(int16 x, int16 y);
static void guiOnClick(uint8 mbutton, bool down);

bool GUI::init() {
  keOnMouseMove.subscribe(guiOnMove);
  keOnMouseButton.subscribe(guiOnClick);
  return 1;
}

void GUI::shutdown() {
  keOnMouseButton.unsubscribe(guiOnClick);
  keOnMouseMove.unsubscribe(guiOnMove);
}

Button* GUI::createButton(uint32 x, uint32 y, uint32 width, uint32 height, uint32 radius, vec3 color) {
  Button* handle = new Button(x, y, width, height, radius, color);
  buttons.insert(handle);
}

void GUI::addButton(Button* button) {
  buttons.insert(button);
}

void GUI::removeButton(Button* button) {
  buttons.erase(button);
}

void GUI::destroyButton(Button* button) {
  buttons.erase(button);
  delete button;
}

void guiOnMove(int16 x, int16 y) {
  if (InputSystem::getCapture()) return;
  bool hit = 0;
  for (Button* button : buttons) {
    hit = button->isHit(x, y);
    if (hit && !button->hovered) {
      if (button->onHover) button->onHover(button);
      button->hovered = true;
    } else if (!hit && button->hovered) {
      if (button->onUnhover) button->onUnhover(button);
      button->hovered = false;
    }
  }
}

void guiOnClick(uint8 mbutton, bool down) {
  if (InputSystem::getCapture()) return;
  int16 x, y;
  InputSystem::getMousePos(x, y);
  for (Button* button : buttons) {
    if (button->isHit(x, y)) {
      if (down) {
        if (button->onPress) button->onPress(button, (ButtonCode)mbutton);
        button->pressed = true;
      }
      else {
        if (button->onRelease) button->onRelease(button, (ButtonCode)mbutton);
        button->pressed = false;
      }
    }
  }
}