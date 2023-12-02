#pragma once

#include <GL/gl.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* WINGLContext;

extern WINGLContext winglCreateContext(HDC hDC);

extern void winglDestroyContext(WINGLContext ctx);

extern int winglMakeCurrent(WINGLContext ctx);

extern void winglSwapBuffers(void); 

#ifdef __cplusplus
}
#endif