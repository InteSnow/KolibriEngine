#include "GUIComponents.h"
#include "renderer/renderer.h"
#include "systems/GUI.h"
#include GL_HEADER

extern uint32 guiShader;

Button::Button(uint32 x, uint32 y, uint32 width, uint32 height, uint32 radius, vec3 color)
 : position{vec2(x, y)}, dimensions{vec2(width, height)}, color{color},
   onHover{NULL}, onUnhover{NULL}, onPress{NULL}, onRelease{NULL} 
{
  this->radius = radius;
  this->hovered = false;
  this->pressed = false;
}

vec2 Button::getPosition() const {
  return this->position;
}

vec2 Button::getDimensions() const {
  return this->dimensions;
}

float Button::getRadius() const {
  return this->radius;
}

vec3 Button::getColor() const {
  return this->color;
}

void Button::setPosition(uint32 x, uint32 y) {
  this->position = vec2(x, y);
}

void Button::setDimensions(uint32 width, uint32 height) {
  this->dimensions = vec2(width, height);
}

void Button::setRadius(uint32 radius) {
  this->radius = radius;
}

void Button::setColor(vec3 color) {
  this->color = color;
}

bool Button::isHit(int16 x, int16 y) const {
  vec2 coords = abs(vec2(x, y) - position - dimensions/2);
  return max(coords - dimensions/2 + radius, vec2(0, 0)).length() <= radius;
}

void Button::onRegister() {
  GUI::addButton(this);
}

void Button::onUnregister() {
  GUI::removeButton(this);
}

void Button::onShapeDraw() {
#ifdef KE_PLATFORM_WIN32
  glUniform2f(glGetUniformLocation(guiShader, "dimensions"), dimensions.x, dimensions.y);
  glUniform1f(glGetUniformLocation(guiShader, "radius"), radius);
#else
  glSetUniform(GL_QUAD_WIDTH, dimensions.x);
  glSetUniform(GL_QUAD_HEIGHT, dimensions.y);
  glSetUniform(GL_RADIUS, radius);
#endif
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glColor3fv(&color.x); glVertex2f(position.x+0,            position.y+dimensions.y);
  glTexCoord2f(1.0f, 0.0f); glColor3fv(&color.x); glVertex2f(position.x+dimensions.x, position.y+dimensions.y);
  glTexCoord2f(1.0f, 1.0f); glColor3fv(&color.x); glVertex2f(position.x+dimensions.x, position.y);
  glTexCoord2f(0.0f, 1.0f); glColor3fv(&color.x); glVertex2f(position.x+0,            position.y);
  glEnd();

}
