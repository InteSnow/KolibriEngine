#include "Widgets.h"
#include "renderer/renderer.h"
#include "systems/GUI.h"

Button::Button() {
  this->hovered = 0;
  for (uint32 i = 0; i < _BUTTON_CODE_MAX_ENUM; i++) {
    this->pressed[i] = 0;
  }
}

bool Button::isHovered() const {
  return this->hovered;
}

bool Button::isPressed(ButtonCode button) const {
  return this->pressed[button];
}

void Button::onHover() {
  this->hovered = true;
}

void Button::onUnhover() {
  this->hovered = false;
}

void Button::onPress(ButtonCode button) {
  this->pressed[button] = 1;
}

void Button::onRelease(ButtonCode button) {
  this->pressed[button] = 0;
}

void Button::onRegister() {
  GUI::addButton(this);
}

void Button::onUnregister() {
  GUI::removeButton(this);
}

Slider::Slider() {

}

void Slider::onRegister() {
  thumb = new Button;
  thumb->container = &thumbRect;
  valText.container = &valRect;
  thumb->onRegister();
}

void Slider::onUnregister() {
  thumb->onRegister();
  delete thumb;
}

void Slider::setRange(vec2 range) {
  minVal = range.x;
  maxVal = range.y;
}

void Slider::setValue(float val) {
  this->val = val;
  thumbX = pos.x + (val - minVal) / (maxVal - minVal);
}

void Slider::setFont(const char* familyName, uint32 fontHeight, uint32 height, vec3 color) {
  Fonts::select(familyName, fontHeight);
  this->valHeight = height;
  this->font = Fonts::getActive();

  valText = Text(&valRect, familyName, fontHeight, height);
}

vec2 Slider::getRange() const {
  return vec2(minVal, maxVal);
}

float Slider::getValue() const {
  return val;
}

const Font* Slider::getFont() const {
  return font;
}
#include "core/logger.h"
void Slider::Update() {
  if (pos.x != container->getPosition().x || pos.y != container->getPosition().y
   || size.x != container->getDimensions().x || size.y != container->getDimensions().y) {
    pos = container->getPosition();
    size = container->getDimensions();
    thumbX = pos.x + (val - minVal) / (maxVal - minVal) * (size.x - size.y);
    thumbRect = Rect(thumbX, pos.y, size.y, size.y, size.y/2, vec4(1.0f));
    track = Rect(pos.x+size.y/2, pos.y+size.y/3, size.x-size.y, size.y/3, size.y/6, vec4(1.0f));
  }

  if (thumb->isPressed(BUTTON_LEFT)) {
    int16 x, y;
    InputSystem::getMousePos(x, y);
    float tempX = clamp((float)x-size.y/2, pos.x, pos.x+size.x-size.y);
    if (tempX == thumbX) return;
    thumbX = tempX;
    val = (thumbX-pos.x) * (maxVal-minVal) / (size.x - size.y) + minVal;
    thumbRect = Rect(thumbX, pos.y, size.y, size.y, size.y/2, vec4(1.0f));
  }
}

void Slider::onShapeDraw() {
  track.onDraw();
  thumbRect.onDraw();
}

void Slider::onTextDraw() {
  if (!valHeight) return;
  valRect = Rect(pos.x+size.x+size.y/2, pos.y, Fonts::getWidth(valHeight, "%.2f", val), valHeight, 0, vec4(0.0f));
  valText.setText("%.2f", val);
  valText.onDraw();
}