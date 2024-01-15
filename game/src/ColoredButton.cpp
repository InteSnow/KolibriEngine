#include "ColoredButton.h"

ColoredButton::ColoredButton(vec4 dc, vec4 hc, vec4 pc)
  : Button(), defaultC {dc}, hoveredC {hc}, pressedC {pc} {}

void ColoredButton::Update() {
  if (pressed[BUTTON_LEFT] && mode != 2) {
    this->container->setColor(pressedC);
    mode = 2;
  } else if (!pressed[BUTTON_LEFT] && hovered && mode != 1) {
    this->container->setColor(hoveredC);
    mode = 1;
  } else if (!pressed[BUTTON_LEFT] && !hovered && mode) {
    this->container->setColor(defaultC);
    mode = 0;
  }
}

