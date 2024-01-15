#pragma once
#include <gui/Widgets.h>

class ColoredButton : public Button {
  void Update() override;
  uint16 mode = 0;
public:

  ColoredButton(vec4 dc = vec4(1.0f), vec4 hc = vec4(vec3(0.9f), 1.0f), vec4 pc = vec4(vec3(0.8f), 1.0f));

  vec4 defaultC;
  vec4 hoveredC;
  vec4 pressedC;
};