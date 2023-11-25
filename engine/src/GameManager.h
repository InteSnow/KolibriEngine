#pragma once
#include "defines.h"

struct GameManager {
  void init(void);

  void update(void);
  void render(void);

  void shutdown(void);
};