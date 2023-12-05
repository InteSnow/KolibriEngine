#include "GameObject.h"

void GameObject::destroy(GameObject& obj) {
  for (KEComponent* c : obj.components) {
    c->onUnregister();
    delete c;
  }
  obj.components.clear();
}

void GameObject::onRenderBegin() {
  transform.push();

  for (KEComponent* c : this->components) {
    c->onRenderBegin();
  }
  transform.pop();
}

void GameObject::onDraw() {
  transform.push();

  for (KEComponent* c : this->components) {
    c->onDraw();
  }

  transform.pop();
}

void GameObject::onRenderEnd() {
  transform.push();

  for (KEComponent* c : this->components) {
    c->onRenderEnd();
  }

  transform.pop();
}