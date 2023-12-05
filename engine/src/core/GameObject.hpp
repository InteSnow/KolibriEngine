#include "GameObject.h"
#include <type_traits>
#include <typeinfo>
#include <tuple>

template <int I, class... Ts>
typename std::enable_if<I < sizeof...(Ts), void>::type
_addAll(GameObject* obj) {
  using T = typename std::tuple_element<I,std::tuple<Ts...>>::type;
  static_assert(std::is_base_of<KEComponent, T>::value, "Component is not derived from KEComponent");
  obj->add<T>();
  _addAll<I+1,Ts...>(obj);
}

template <int I, class... Ts>
typename std::enable_if<I == sizeof...(Ts), void>::type
_addAll(GameObject* obj) {

}

template <class... Ts>
GameObject* GameObject::create() {
  GameObject* obj = new GameObject;
  obj->transform = Transform();
  _addAll<0, Ts...>(obj);

  GameObject::objects.insert(obj);
  return obj;
}

template <class T>
T* GameObject::get() {
  static_assert(std::is_base_of<KEComponent, T>::value, "Component is not derived from KEComponent");
  
  for (KEComponent* c : this->components) {
    if (typeid(*c) == typeid(T)) {
      return (T*)c;
    }
  }
  return NULL;
}

template <class T>
void GameObject::add() {
  static_assert(std::is_base_of<KEComponent, T>::value, "Component is not derived from KEComponent");
  if (this->get<T>()) return;
  KEComponent* component = new T;
  component->onRegister();
  this->components.insert(component);
}

template <class T>
void GameObject::remove(void) {
  static_assert(std::is_base_of<KEComponent, T>::value, "Component is not derived from KEComponent");
  KEComponent* comp = this->get<T>();
  comp->onUnregister();
  delete comp;
  this->components.erase(comp);
}