#include "Object.h"

std::unordered_set<SceneObject*> SceneObject::objects;

void SceneObject::destroyNoErase(SceneObject* obj) {
  for (SceneComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  delete obj;
}

void SceneObject::destroy(SceneObject* obj) {
  for (SceneComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  SceneObject::objects.erase(obj);
  delete obj;
}

void SceneObject::makeActive() {
  this->active = true;
}

void SceneObject::makeInactive() {
  this->active = false;
}

bool SceneObject::isActive() {
  return this->active;
}

void SceneObject::onRenderBegin() {
  if (!this->active) return;

  transform.push();

  for (SceneComponent* c : this->components) {
    c->onRenderBegin();
  }
  transform.pop();
}

void SceneObject::onDraw() {
  if (!this->active) return;

  transform.push();

  for (SceneComponent* c : this->components) {
    c->onDraw();
  }

  transform.pop();
}

void SceneObject::onRenderEnd() {
  if (!this->active) return;
  
  transform.push();

  for (SceneComponent* c : this->components) {
    c->onRenderEnd();
  }

  transform.pop();
}

std::unordered_set<GUIObject*> GUIObject::objects;

void GUIObject::destroyNoErase(GUIObject* obj) {
  for (GUIComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  delete obj;
}

void GUIObject::destroy(GUIObject* obj) {
  for (GUIComponent* c : obj->components) {
    c->onUnregister();
    delete c;
  }
  obj->components.clear();
  GUIObject::objects.erase(obj);
  delete obj;
}

void GUIObject::makeActive() {
  this->active = true;
}

void GUIObject::makeInactive() {
  this->active = false;
}

bool GUIObject::isActive() {
  return this->active;
}

void GUIObject::onShapeDraw() {
  if (!this->active) return;

  for (GUIComponent* c : this->components) {
    c->onShapeDraw();
  }
}

void GUIObject::onTextDraw() {
  if (!this->active) return;

  for (GUIComponent* c : this->components) {
    c->onTextDraw();
  }
}