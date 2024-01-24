#pragma once
#include "defines.h"
#include "physics/Collider.h"
#include "physics/RigidBody.h"
#include <unordered_set>

class Physics {
  static std::unordered_set<Collider*> colliders;
  static std::unordered_set<RigidBody*> rigidBodies;

public:
  static float g;

  static void addCollider(Collider* collider);
  static void removeCollider(Collider* collider);
  static void addRigidBody(RigidBody* body);
  static void removeRigidBody(RigidBody* body);
  static void update(void);
};
