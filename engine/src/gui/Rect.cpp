#include "Rect.h"
#include GL_HEADER

extern uint32 guiShader;

vec2 Rect::getPosition() const {
  return this->position;
}

vec2 Rect::getDimensions() const {
  return this->dimensions;
}

float Rect::getRadius() const {
  return this->radius;
}

vec4 Rect::getColor() const {
  return this->color;
}

void Rect::setPosition(uint32 x, uint32 y) {
  this->position = vec2(x, y);
}

void Rect::setDimensions(uint32 width, uint32 height) {
  this->dimensions = vec2(width, height);
}

void Rect::setRadius(uint32 radius) {
  this->radius = radius;
}

void Rect::setColor(vec4 color) {
  this->color = color;
}

bool Rect::isHit(int16 x, int16 y) const {
  vec2 coords = abs(vec2(x, y) - position - dimensions/2);
  return max(coords - dimensions/2 + radius, vec2(0, 0)).length() <= radius;
}

void Rect::onDraw(void) {
  if (color.w == 0) return;
#ifdef KE_PLATFORM_WIN32
  glUniform2f(glGetUniformLocation(guiShader, "dimensions"), dimensions.x, dimensions.y);
  glUniform1f(glGetUniformLocation(guiShader, "radius"), radius);
  //glSetUniform(GL_QUAD_WIDTH, dimensions.x);
  //glSetUniform(GL_QUAD_HEIGHT, dimensions.y);
  //glSetUniform(GL_RADIUS, radius);
#else
  glSetUniform(GL_QUAD_WIDTH, dimensions.x);
  glSetUniform(GL_QUAD_HEIGHT, dimensions.y);
  glSetUniform(GL_RADIUS, radius);
#endif
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f); glColor4fv(&color.x); glVertex2f(position.x+0,            position.y+dimensions.y);
  glTexCoord2f(1.0f, 0.0f); glColor4fv(&color.x); glVertex2f(position.x+dimensions.x, position.y+dimensions.y);
  glTexCoord2f(1.0f, 1.0f); glColor4fv(&color.x); glVertex2f(position.x+dimensions.x, position.y);
  glTexCoord2f(0.0f, 1.0f); glColor4fv(&color.x); glVertex2f(position.x+0,            position.y);
  glEnd();
}

Rect::Rect(uint32 x, uint32 y, uint32 width, uint32 height, uint32 radius, vec4 color)
 : position{vec2(x, y)}, dimensions{vec2(width, height)}, color{color}
{
  this->radius = radius;
}