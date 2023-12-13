#include "GameObject.h"

std::unordered_set<GameObject*> GameObject::objects;

void GameObject::destroyNoErase(GameObject* obj) {
  for (KEComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  delete obj;
}

void GameObject::destroy(GameObject* obj) {
  for (KEComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  GameObject::objects.erase(obj);
  delete obj;
}

void GameObject::makeActive() {
  this->active = true;
}

void GameObject::makeInactive() {
  this->active = false;
}

bool GameObject::isActive() {
  return this->active;
}

void GameObject::onRenderBegin() {
  if (!this->active) return;

  transform.push();

  for (KEComponent* c : this->components) {
    c->onRenderBegin();
  }
  transform.pop();
}

void GameObject::onDraw() {
  if (!this->active) return;

  transform.push();

  for (KEComponent* c : this->components) {
    c->onDraw();
  }

  transform.pop();
}

void GameObject::onRenderEnd() {
  if (!this->active) return;
  
  transform.push();

  for (KEComponent* c : this->components) {
    c->onRenderEnd();
  }

  transform.pop();
}