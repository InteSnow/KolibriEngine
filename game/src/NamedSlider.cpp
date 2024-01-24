#include "NamedSlider.h"
#include <cstdarg>

void NamedSlider::onRegister() {
  name.container = &nameRect;
  Slider::onRegister();
}

void NamedSlider::setName(const wchar_t* format, ...) {
  va_list vl;
  va_start(vl, format);
  name.setText(format, vl);
  va_end(vl);
}

void NamedSlider::onTextDraw() {
  vec2 pos = container->getPosition() + nameOffset;
  if (nameRect.getPosition().x != pos.x || nameRect.getPosition().y != pos.y) {
    nameRect = Rect(pos.x, pos.y, size.x, size.y, 0, vec4(0.0f));
  }
  name.familyName = font->familyName;
  name.fontHeight = font->height;
  name.height = valHeight;
  name.color = valColor;
  name.onDraw();
  Slider::onTextDraw();
}