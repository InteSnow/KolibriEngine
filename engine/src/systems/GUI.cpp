#include "GUI.h"
#include "core/Events.h"
#include "core/Object.h"

static std::unordered_set<Button*> buttons;

static void guiOnMove(int16 x, int16 y);
static void guiOnClick(uint8 mbutton, bool down);

static bool captureChanged = 0;

bool GUI::init() {
  keOnMouseMove.subscribe(guiOnMove);
  keOnMouseButton.subscribe(guiOnClick);
  return 1;
}

void GUI::shutdown() {
  keOnMouseButton.unsubscribe(guiOnClick);
  keOnMouseMove.unsubscribe(guiOnMove);
}

void GUI::addButton(Button* button) {
  buttons.insert(button);
}

void GUI::removeButton(Button* button) {
  buttons.erase(button);
}

void guiOnMove(int16 x, int16 y) {
  if (InputSystem::getCapture()) {
    if (captureChanged) {
      for (Button* button : buttons) {
        for (uint32 i = 0; i < _BUTTON_CODE_MAX_ENUM; i++) {
          if (button->isPressed((ButtonCode)i))
            button->onRelease((ButtonCode)i);
        }
        if (button->isHovered())
          button->onUnhover();
      }
      captureChanged = 0;
    }
    return;
  }
  captureChanged = 1;
  
  bool hit = 0;
  for (Button* button : buttons) {
    hit = button->container->isHit(x, y);
    if (hit && !button->isHovered()) {
      button->onHover();
    } else if (!hit && button->isHovered()) {
      button->onUnhover();
    }
  }
}

void guiOnClick(uint8 mbutton, bool down) {
  if (InputSystem::getCapture()) return;

  int16 x, y;
  InputSystem::getMousePos(x, y);
  for (Button* button : buttons) {
    if (button->container->isHit(x, y) && down) {
      button->onPress((ButtonCode)mbutton);
    } else if (button->isPressed((ButtonCode)mbutton) && !down) {
      button->onRelease((ButtonCode)mbutton);
    }
  }
}