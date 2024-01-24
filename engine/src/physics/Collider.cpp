#include "Collider.h"
#include "systems/Physics.h"

Collider::Collider(const BoundingBox& box) 
  : box{box} {}

void Collider::onRegister() {
  Physics::addCollider(this);
}

void Collider::onUnregister() {
  Physics::removeCollider(this);
}

bool Collider::collides(const Collider* other) {
  if (!this->parent || !other->parent) return 0;
  BoundingBox tb = {this->box.min*this->parent->transform.size+this->parent->transform.position,
                    this->box.max*this->parent->transform.size+this->parent->transform.position};
  BoundingBox ob = {other->box.min*other->parent->transform.size+other->parent->transform.position,
                    other->box.max*other->parent->transform.size+other->parent->transform.position};
  
  return tb.min.x < ob.max.x && tb.max.x > ob.min.x
      && tb.min.y < ob.max.y && tb.max.y > ob.min.y
      && tb.min.z < ob.max.z && tb.max.z > ob.min.z;
}

vec3 Collider::getCollisionDir(const Collider* other) {
  if (!this->parent || !other->parent) return 0;
  BoundingBox tb = {this->box.min*this->parent->transform.size+this->parent->transform.position,
                    this->box.max*this->parent->transform.size+this->parent->transform.position};
  BoundingBox ob = {other->box.min*other->parent->transform.size+other->parent->transform.position,
                    other->box.max*other->parent->transform.size+other->parent->transform.position};
  float dx1 = ob.max.x - tb.min.x, dx2 = tb.max.x - ob.min.x,
        dy1 = ob.max.y - tb.min.y, dy2 = tb.max.y - ob.min.y,
        dz1 = ob.max.z - tb.min.z, dz2 = tb.max.z - ob.min.z;
  if (dx1 < dx2 && dx1 < dy1 && dx1 < dy2 && dx1 < dz1 && dx1 < dz2) 
    return vec3(-1.0f,  0.0f,  0.0f);
  if (dx2 < dy1 && dx2 < dy2 && dx2 < dz1 && dx2 < dz2) 
    return vec3( 1.0f,  0.0f,  0.0f);
  if (dy1 < dy2 && dy1 < dz1 && dy1 < dz2) 
    return vec3( 0.0f, -1.0f,  0.0f);
  if (dy2 < dz1 && dy2 < dz2) 
    return vec3( 0.0f,  1.0f,  0.0f);
  if (dz1 < dz2)
    return vec3( 0.0f,  0.0f, -1.0f);
  return   vec3( 0.0f,  0.0f,  1.0f);
}