#include "ColoredButton.h"

ColoredButton::ColoredButton(vec3 dc, vec3 hc, vec3 pc)
  : Button(), defaultC {dc}, hoveredC {hc}, pressedC {pc} {}

void ColoredButton::Update() {
  if (pressed[BUTTON_LEFT] && mode != 2) {
    this->parent->rect.setColor(pressedC);
    mode = 2;
  } else if (!pressed[BUTTON_LEFT] && hovered && mode != 1) {
    this->parent->rect.setColor(hoveredC);
    mode = 1;
  } else if (!pressed[BUTTON_LEFT] && !hovered && mode) {
    this->parent->rect.setColor(defaultC);
    mode = 0;
  }
}

