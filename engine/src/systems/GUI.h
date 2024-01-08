#pragma once
#include "defines.h"
#include "InputSystem.h"
#include "core/GUIComponents.h"

#include <unordered_set>

class GUI {
public:
  static bool init(void);
  static void shutdown(void);

  static Button* createButton(uint32 x = 0, uint32 y = 0, uint32 width = 0, uint32 height = 0, uint32 radius = 0, vec3 color = vec3(1.0f));
  static void addButton(Button* button);
  static void removeButton(Button* button);
  static void destroyButton(Button* button);
};