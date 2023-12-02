#pragma once
#include "defines.h"

typedef enum ColorFlagBits {
  COLOR_RED = 0x01,
  COLOR_GREEN = 0x02,
  COLOR_BLUE = 0x04,
  COLOR_BRIGHT = 0x08
} ColorFlagBits;
typedef uint32 ColorFlags;

bool platformInit(const char* appName, int32 x, int32 y, int32 width, int32 height);

void platformShutdown(void);

void pollEvents(void);
void platformPresent(void);

uint64 getTime(void);

void consoleWrite(const char* msg, ColorFlags color);

void platformSetCapture(bool mode);

void platformToggleFullscreen(void);