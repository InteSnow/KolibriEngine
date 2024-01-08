#pragma once
#include "defines.h"
#include <vector>
#include <unordered_set>

template<typename... Args>
class Event {
  std::unordered_set<void (*)(Args...)> callbacks;

public:
  void fire(Args... args) const {
    for (void (*callback)(Args...) : callbacks) {
      callback(args...);
    }
  }

  void subscribe(void (*callback)(Args...)) {
    this->callbacks.insert(callback);
  }

  void unsubscribe(void (*callback)(Args...)) {
    this->callbacks.erase(callback);
  }
};

extern Event<> keOnQuit;
extern Event<uint8, bool> keOnKey;
extern Event<uint8, bool> keOnMouseButton;
extern Event<int16, int16> keOnMouseMove;
extern Event<int8> keOnMouseWheel;
extern Event<uint16, uint16> keOnResize;