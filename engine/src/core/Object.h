#pragma once
#include "defines.h"
#include "core/Transform.h"
#include <unordered_set>

class SceneObject {
  bool active = true;

  std::unordered_set<SceneComponent*> components;

  static std::unordered_set<SceneObject*> objects;

  friend class EngineLoop;
  
  static void destroyNoErase(SceneObject* obj);
public:
  Transform transform = Transform();

  template <class... Ts>
  static SceneObject* create(void);

  static void destroy(SceneObject* obj);

  template <class T>
  T* get(void);

  template <class T>
  void add(void);

  template <class T>
  void remove(void);

  void makeActive(void);
  void makeInactive(void);
  bool isActive(void);

  void onRenderBegin(void);
  void onDraw(void);
  void onRenderEnd(void);
};

#define OBJECT_TYPE SceneObject
#define COMPONENT_TYPE SceneComponent
#include "Object.hpp"
#undef COMPONENT_TYPE
#undef OBJECT_TYPE

class GUIObject {
  bool active = true;

  std::unordered_set<GUIComponent*> components;

  static std::unordered_set<GUIObject*> objects;

  friend class EngineLoop;
  
  static void destroyNoErase(GUIObject* obj);
public:
  template <class... Ts>
  static GUIObject* create(void);

  static void destroy(GUIObject* obj);

  template <class T>
  T* get(void);

  template <class T>
  void add(void);

  template <class T>
  void remove(void);

  void makeActive(void);
  void makeInactive(void);
  bool isActive(void);

  void onShapeDraw(void);
  void onTextDraw(void);
};

#define OBJECT_TYPE GUIObject
#define COMPONENT_TYPE GUIComponent
#include "Object.hpp"
#undef COMPONENT_TYPE
#undef OBJECT_TYPE