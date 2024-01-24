#pragma once
#include "defines.h"
#include "core/Component.h"
#include "core/Object.h"
#include <vector>

class Physics;

struct BoundingBox {
  vec3 min;
  vec3 max;
};

class Collider : public SceneComponent {


  friend Physics; 

  void onRegister(void) override final;
  void onUnregister(void) override final;

protected:
  virtual void onCollision(const Collider* other) {};

public:
  BoundingBox box;

  Collider() {};
  Collider(const BoundingBox& box);
  bool collides(const Collider* other);
  vec3 getCollisionDir(const Collider* other);
};