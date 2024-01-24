#include "RigidBody.h"
#include "core/Transform.h"
#include "systems/Physics.h"
#include "core/logger.h"
RigidBody::RigidBody(const BoundingBox& box) : box{box} {}

void RigidBody::setBoundingBox(const BoundingBox& box) {
  this->box = box;
  this->collider->box = box;
}

void RigidBody::onRegister() {
  this->collider = new Collider(this->box);
  this->collider->parent = this->parent;
  Physics::addRigidBody(this);
}

void RigidBody::onUnregister() {
  Physics::removeRigidBody(this);
  this->collider->parent = this->parent;
  delete this->collider;
}

void RigidBody::Update() {
  if (immovable) {
    impulse = 0;
    force = 0;
    return;
  }

  if (updateImpulse) {
    impulse = nextImpulse;
    updateImpulse = 0;
  }

  float t = Time->deltaTime;
  parent->transform.move(impulse/mass*t + force/mass/2.0f*t*t);
  impulse = impulse + force*t;

  force = vec3(0.0f);
}

void RigidBody::setImpulse(vec3 impulse) {
  if (immovable) return;
  this->updateImpulse = 1;
  this->nextImpulse = impulse;
}

void RigidBody::applyForce(vec3 force) {
  if (immovable) return;
  this->force = this->force + force;
}

void RigidBody::onCollision(const RigidBody* other) {
  if (this->immovable) return;
  
  BoundingBox tb = {this->box.min*this->parent->transform.size+this->parent->transform.position,
                    this->box.max*this->parent->transform.size+this->parent->transform.position};
  BoundingBox ob = {other->box.min*other->parent->transform.size+other->parent->transform.position,
                    other->box.max*other->parent->transform.size+other->parent->transform.position};

  vec3 dir = this->collider->getCollisionDir(other->collider);
  float k = (this->restitution+other->restitution)/2.0f;
  vec3 newImpulse = impulse;
  if (dir.x) {
    if (dir.x > 0) {
      if (this->impulse.x > 0) this->parent->transform.position.x += ob.min.x - tb.max.x;
    } else {
      if (this->impulse.x < 0) this->parent->transform.position.x += ob.max.x - tb.min.x;
    }
    if (!other->immovable)
      newImpulse.x -= (1+k)*(other->mass*this->impulse.x-this->mass*other->impulse.x)/(this->mass+other->mass);
    else
      newImpulse.x -= (1+k)*this->impulse.x;
  } else if (dir.y) {
    if (dir.y > 0) {
      if (this->impulse.y > 0) this->parent->transform.position.y += ob.min.y - tb.max.y;
    } else {
      if (this->impulse.y < 0) this->parent->transform.position.y += ob.max.y - tb.min.y;
    }
    if (!other->immovable)
      newImpulse.y -= (1+k)*(other->mass*this->impulse.y-this->mass*other->impulse.y)/(this->mass+other->mass);
    else 
      newImpulse.y -= (1+k)*this->impulse.y;
  } else {
    if (dir.z > 0) {
      if (this->impulse.z > 0) this->parent->transform.position.z += ob.min.z - tb.max.z;
    } else {
      if (this->impulse.z < 0) this->parent->transform.position.z += ob.max.z - tb.min.z;
    }
    if (!other->immovable)
      newImpulse.z -= (1+k)*(other->mass*this->impulse.z-this->mass*other->impulse.z)/(this->mass+other->mass);
    else
      newImpulse.z -= (1+k)*this->impulse.z;
  }

  if (newImpulse.x < 0.001f && newImpulse.x > -0.001f) newImpulse.x = 0.0f;
  if (newImpulse.y < 0.001f && newImpulse.y > -0.001f) newImpulse.y = 0.0f;
  if (newImpulse.z < 0.001f && newImpulse.z > -0.001f) newImpulse.z = 0.0f;

  vec3 frictionF = vec3(0.0f);
  if (!dir.x) {
    frictionF.x = -(this->friction+other->friction)/2.0f * mass * Physics::g;
    if (newImpulse.x) {
      if (newImpulse.x > 0) frictionF.x *= -1;
    } else if (force.x > 0) {
      newImpulse.x = 0.0f;
      frictionF.x *= -1;
      if (force.x < -frictionF.x) {
        frictionF.x = -force.x;
      }
    } else if (force.x > -frictionF.x) {
      newImpulse.x = 0.0f;
      frictionF.x = -force.x;
    }
  }
  if (!dir.y) {
    frictionF.y = -(this->friction+other->friction)/2.0f * mass * Physics::g;
    if (newImpulse.y) {
      if (newImpulse.y > 0) frictionF.y *= -1;
    } else if (force.y > 0) {
      newImpulse.y = 0.0f;
      frictionF.y *= -1;
      if (force.y < -frictionF.y) {
        frictionF.y = -force.y;
      }
    } else if (force.y > -frictionF.y) {
      newImpulse.y = 0.0f;
      frictionF.y = -force.y;
    }
  }
  if (!dir.z) {
    frictionF.z = -(this->friction+other->friction)/2.0f * mass * Physics::g;
    if (newImpulse.z) {
      if (newImpulse.z > 0) frictionF.z *= -1;
    } else if (force.z > 0) {
      newImpulse.z = 0.0f;
      frictionF.z *= -1;
      if (force.z < -frictionF.z) {
        frictionF.z = -force.z;
      }
    } else if (force.z > -frictionF.z) {
      newImpulse.z = 0.0f;
      frictionF.z = -force.z;
    }
  }
  setImpulse(newImpulse);
  applyForce(frictionF);
}