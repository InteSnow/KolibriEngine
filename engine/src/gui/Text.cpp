#include "Text.h"
#include "systems/Fonts.h"

Text::Text(Rect* container, const char* familyName, uint32 fontHeight, uint32 height, vec3 color)
  : container{container}, familyName{familyName}, fontHeight{fontHeight}, height{height}, color{color} {}

void Text::setText(const char* format, ...) {
  va_list vl;
  va_start(vl, format);
  this->text = sformat(format, vl);
  va_end(vl);
}

void Text::onDraw() {
  if (!fontHeight || !familyName) return;

  if (strcmp(Fonts::getActive()->familyName, familyName) || Fonts::getActive()->height != fontHeight) {
    Fonts::select(familyName, fontHeight);
  }
  
  uint32 twidth = Fonts::getWidth(height, "%s", this->text.c_str());
  uint32 theight = Fonts::getActive()->median*2;

  Fonts::print(
    container->getPosition().x + (container->getDimensions().x-twidth) / 2,
    container->getPosition().y + (container->getDimensions().y-theight) / 2,
    height, color, "%s", this->text.c_str()
  );
}
