#include "Object.h"
#include <type_traits>
#include <typeinfo>
#include <tuple>

template <int I, class... Ts>
typename std::enable_if<I < sizeof...(Ts), void>::type
_addAll(OBJECT_TYPE* obj, Ts... args) {
  using T = typename std::tuple_element<I,std::tuple<Ts...>>::type;
  static_assert(std::is_base_of<COMPONENT_TYPE, T>::value, "Component is not derived from COMPONENT_TYPE");
  obj->add<T>(std::get<I>(std::make_tuple<Ts...>(std::move(args)...)));
  _addAll<I+1,Ts...>(obj);
}

template <int I, class... Ts>
typename std::enable_if<I == sizeof...(Ts), void>::type
_addAll(OBJECT_TYPE* obj) {

}

template <class... Ts>
OBJECT_TYPE* OBJECT_TYPE::create(Ts... args) {
  OBJECT_TYPE* obj = new OBJECT_TYPE;
  _addAll<0, Ts...>(obj, args...);

  OBJECT_TYPE::objects.insert(obj);
  return obj;
}

template <class T>
T* OBJECT_TYPE::get() {
  static_assert(std::is_base_of<COMPONENT_TYPE, T>::value, "Component is not derived from COMPONENT_TYPE");
  
  for (COMPONENT_TYPE* c : this->components) {
    if (typeid(*c) == typeid(T)) {
      return (T*)c;
    }
  }
  return NULL;
}

template <class T>
void OBJECT_TYPE::add(T arg) {
  static_assert(std::is_base_of<COMPONENT_TYPE, T>::value, "Component is not derived from COMPONENT_TYPE");
  if (this->get<T>()) return;
  COMPONENT_TYPE* component = new T(arg);
  component->parent = this;
  component->onRegister();
  this->components.insert(component);
}

template <class T>
void OBJECT_TYPE::remove(void) {
  static_assert(std::is_base_of<COMPONENT_TYPE, T>::value, "Component is not derived from COMPONENT_TYPE");
  COMPONENT_TYPE* comp = this->get<T>();
  comp->onUnregister();
  delete comp;
  this->components.erase(comp);
}