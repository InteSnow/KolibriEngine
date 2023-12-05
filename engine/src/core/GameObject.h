#pragma once
#include "defines.h"
#include "core/Transform.h"
#include <unordered_set>

class GameObject {
  std::unordered_set<KEComponent*> components;

  static std::unordered_set<GameObject*> objects;

  friend class EngineLoop;
  
  static void destroyNoErase(GameObject* obj);
public:
  Transform transform;

  template <class... Ts>
  static GameObject* create(void);

  static void destroy(GameObject* obj);

  template <class T>
  T* get(void);

  template <class T>
  void add(void);

  template <class T>
  void remove(void);

  void onRenderBegin(void);
  void onDraw(void);
  void onRenderEnd(void);
};

#include "GameObject.hpp"