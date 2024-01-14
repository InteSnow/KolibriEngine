#pragma once
#include "defines.h"
#include "InputSystem.h"
#include "gui/Widgets.h"

#include <unordered_set>

class GUI {
public:
  static bool init(void);
  static void shutdown(void);

  static void addButton(Button* button);
  static void removeButton(Button* button);
};

