#pragma once

// integer types
typedef unsigned char       uint8;
typedef char                int8;
typedef unsigned short      uint16;
typedef short               int16;
typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned long long  uint64;
typedef long long           int64;

// floating point types
typedef float   float32;
typedef double  float64;

#ifdef KOS32
#define KE_PLATFORM_KOS32
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define KE_PLATFORM_WIN32
#else 
#error "Current OS is not supported!"
#endif

#ifdef KE_PLATFORM_KOS32
#define GL_GLEXT_PROTOTYPES 1
#define GL_HEADER <GL/gl.h> 
#elif defined(KE_PLATFORM_WIN32)
#define GLEW_STATIC
#define GL_HEADER <GL/glew.h>
#endif

struct TimeInfo {
  float32 deltaTime;
  float32 time;
};

extern const TimeInfo* Time;