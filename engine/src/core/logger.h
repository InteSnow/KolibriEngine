#pragma once
#include "defines.h"

#ifdef _DEBUG
#define KE_DEBUG_ENABLE
#endif

#define KE_INFO_ENABLE
#define KE_WARN_ENABLE

typedef enum KeLogVerbosity {
  LOG_DEBUG = 0,
  LOG_INFO = 1,
  LOG_WARN = 2,
  LOG_ERROR = 3
} KeLogVerbosity;

void keLog(const char* msg, KeLogVerbosity severity, ...);

#ifdef KE_DEBUG_ENABLE
#define KE_DEBUG(msg, ...) keLog(msg, LOG_DEBUG, ##__VA_ARGS__)
#else
#define KE_DEBUG(msg, ...) 
#endif

#ifdef KE_INFO_ENABLE
#define KE_INFO(msg, ...) keLog(msg, LOG_INFO, ##__VA_ARGS__)
#else
#define KE_INFO(msg, ...)
#endif

#ifdef KE_WARN_ENABLE
#define KE_WARN(msg, ...) keLog(msg, LOG_WARN, ##__VA_ARGS__)
#else
#define KE_WARN(msg, ...) 
#endif

#define KE_ERROR(msg, ...) keLog(msg, LOG_ERROR, ##__VA_ARGS__)