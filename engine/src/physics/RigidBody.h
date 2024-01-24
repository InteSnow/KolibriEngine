#pragma once
#include "defines.h"
#include "core/Component.h"
#include "physics/Collider.h"

class RigidBody : public SceneComponent {
  friend Physics;

  void onRegister(void) override final;
  void onUnregister(void) override final;

  BoundingBox box;

  bool updateImpulse;
  vec3 nextImpulse;
  vec3 impulse = vec3(0.0f);
  vec3 force = vec3(0.0f);

protected:
  void Update(void) override;
  virtual void onCollision(const RigidBody* other);
public:
  float restitution = 1.0f;
  float friction = 1.0f;
  float mass = 1.0f;
  bool immovable = 0;

  RigidBody(void) {};
  RigidBody(const BoundingBox& box);
  void setBoundingBox(const BoundingBox& box);
  void setImpulse(vec3 impulse);
  void applyForce(vec3 force);
  Collider* collider;
};