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

vec3 Rect::getColor() const {
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

void Rect::setColor(vec3 color) {
  this->color = color;
}

bool Rect::isHit(int16 x, int16 y) const {
  vec2 coords = abs(vec2(x, y) - position - dimensions/2);
  return max(coords - dimensions/2 + radius, vec2(0, 0)).length() <= radius;
}

void Rect::onShapeDraw(void) {
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
  glTexCoord2f(0.0f, 0.0f); glColor3fv(&color.x); glVertex2f(position.x+0,            position.y+dimensions.y);
  glTexCoord2f(1.0f, 0.0f); glColor3fv(&color.x); glVertex2f(position.x+dimensions.x, position.y+dimensions.y);
  glTexCoord2f(1.0f, 1.0f); glColor3fv(&color.x); glVertex2f(position.x+dimensions.x, position.y);
  glTexCoord2f(0.0f, 1.0f); glColor3fv(&color.x); glVertex2f(position.x+0,            position.y);
  glEnd();
}

void Rect::onTextDraw(void) {

}

Rect::Rect(uint32 x, uint32 y, uint32 width, uint32 height, uint32 radius, vec3 color)
 : position{vec2(x, y)}, dimensions{vec2(width, height)}, color{color}
{
  this->radius = radius;
}