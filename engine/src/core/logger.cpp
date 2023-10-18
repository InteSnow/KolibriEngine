#include "logger.h"
#include "platform/platform.h"

#include <cstdarg>
#include <cstdio>
#include <cstring>

static ColorFlags colors[] = {
  COLOR_RED | COLOR_GREEN | COLOR_BLUE,
  COLOR_RED | COLOR_GREEN | COLOR_BLUE | COLOR_BRIGHT,
  COLOR_RED | COLOR_GREEN | COLOR_BRIGHT,
  COLOR_RED | COLOR_BRIGHT
};

static const char* verbosityStrings[] = {
  "[DEBUG] ", "[INFO] ", "[WARN] ", "[ERROR] "
};

void keLog(const char* msg, KeLogVerbosity severity, ...) {
  uint32 pos = strlen(verbosityStrings[severity]);
  uint32 size = pos;
  
  va_list args;
  va_start(args, severity);

  size += vsnprintf(NULL, 0, msg, args);

  char log[size+2] = {};

  memcpy(log, verbosityStrings[severity], pos);
  vsprintf(log+pos, msg, args);
  log[size] = '\n';

  va_end(args);

  consoleWrite(log, colors[severity]);
}