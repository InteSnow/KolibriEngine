#pragma once
#include <gui/Widgets.h>

class ColoredButton : public Button {
  void Update() override;
  uint16 mode = 0;
public:

  ColoredButton(vec3 dc = vec3(1.0f), vec3 hc = vec3(0.9f), vec3 pc = vec3(0.8f));

  vec3 defaultC;
  vec3 hoveredC;
  vec3 pressedC;
};