#pragma once
#include <gui/Widgets.h>

class NamedSlider : public Slider {
protected:
  void onRegister() override;
  void onTextDraw() override;

  Rect nameRect;
  Text name;
public:
  NamedSlider() : Slider() {}
  vec2 nameOffset = vec2(0.0f);
  void setName(const wchar_t* format, ...);
};