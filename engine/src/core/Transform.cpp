#include "Transform.h"
#include GL_HEADER

Transform::Transform() : position(0.0f), size(1.0f), rotation(0.0f) {}

mat4 Transform::getModel() {
  return ::translate(::rotate(::scale(mat4(1.0f), this->size), rotation), position);
}

void Transform::move(vec3 translation) {
  this->position = this->position + translation;
}

void Transform::scale(vec3 factor) {
  this->size = this->size * factor;
}

void Transform::rotate(vec3 rotation) {
  this->rotation = this->rotation + rotation;
  this->rotation.x = this->rotation.x-((int)this->rotation.x/360)*360;
  this->rotation.y = this->rotation.y-((int)this->rotation.y/360)*360;
  this->rotation.z = this->rotation.z-((int)this->rotation.z/360)*360;
}

void Transform::push() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMultMatrixf(this->getModel().data());
}

void Transform::pop() {
  glPopMatrix();
}