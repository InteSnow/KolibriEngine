#include "Transform.h"
#include GL_HEADER

Transform::Transform() : position(0.0f), size(1.0f), rotation(0.0f) {}

mat4 Transform::getModel() {
  return ::translate(::rotate(::scale(mat4(1.0f), this->size), rotation.length(), rotation), position);
}

void Transform::move(vec3 translation) {
  this->position = this->position + translation;
}

void Transform::scale(vec3 factor) {
  this->size = this->size * factor;
}

void Transform::rotate(vec3 rotation) {
  this->rotation = this->rotation + rotation;
}

void Transform::push() {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMultMatrixf(this->getModel().data());
}

void Transform::pop() {
  glPopMatrix();
}