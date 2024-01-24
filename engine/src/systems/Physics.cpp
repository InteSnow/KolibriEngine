#include "Physics.h"

std::unordered_set<Collider*> Physics::colliders;
std::unordered_set<RigidBody*> Physics::rigidBodies;

float Physics::g  = -9.8f;

void Physics::addCollider(Collider* collider) {
  colliders.insert(collider);
}

void Physics::removeCollider(Collider* collider) {
  colliders.erase(collider);
}

void Physics::addRigidBody(RigidBody* body) {
  rigidBodies.insert(body);
}

void Physics::removeRigidBody(RigidBody* body) {
  rigidBodies.erase(body);
}

void Physics::update() {
  {
  Collider *first, *second;
  for (auto fi = colliders.begin(); fi != colliders.end(); fi++) {
    first = *fi;
    if (!first->parent || !first->parent->isActive()) continue;
    for (auto si = fi; si != colliders.end(); si++) {
      second = *si;
      if (first == second) continue;
      if (!second->parent || !second->parent->isActive()) continue;
      if (first->collides(second)) { 
        first->onCollision(second);
        second->onCollision(first);
      }
    }
  }
  }

  {
  RigidBody *first, *second;

  for (auto fi = rigidBodies.begin(); fi != rigidBodies.end(); fi++) {
    first = *fi;
    if (!first->parent || !first->parent->isActive()) continue;
    first->applyForce(vec3(0.0f, first->mass*g, 0.0f));
  }  
  for (auto fi = rigidBodies.begin(); fi != rigidBodies.end(); fi++) {
    first = *fi;
    if (!first->parent || !first->parent->isActive()) continue;
    for (auto si = fi; si != rigidBodies.end(); si++) {
      second = *si;
      if (first == second) continue;
      if (!second->parent || !second->parent->isActive()) continue;
      if (first->collider->collides(second->collider)) { 
        first->onCollision(second);
        second->onCollision(first);
      }
    }
  }
  }
}