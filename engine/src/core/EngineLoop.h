#pragma once
#include "GameManager.h"

struct GameManager;

class EngineLoop {
  GameManager gameManager;

public:

  bool init(void);

  bool tick(void);

  void exit(void);
};

extern bool shouldExit;