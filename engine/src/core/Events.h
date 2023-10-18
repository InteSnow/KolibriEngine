#pragma once
#include "defines.h"
#include <vector>
#include "core/logger.h"

template<typename... Args>
class Event {
  std::vector<void (*)(Args...)> callbacks;

public:
  void fire(Args... args) const {
    for (int i = 0; i < this->callbacks.size(); i++) {
      callbacks[i](args...);
    }
  }

  void subscribe(void (*callback)(Args...)) {
    this->callbacks.push_back(callback);
  }
};

extern Event<> keOnQuit;
extern Event<uint8, bool> keOnKey;
extern Event<uint8, bool> keOnMouseButton;
extern Event<int16, int16> keOnMouseMove;
extern Event<int8> keOnMouseWheel;
extern Event<uint16, uint16> keOnResize;