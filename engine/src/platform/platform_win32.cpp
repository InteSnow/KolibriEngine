#include "platform.h"

#ifdef KE_PLATFORM_WIN32

#include "systems/InputSystem.h"
#include "core/Events.h"
#include "renderer/renderer.h"

#include <windows.h>
#include <windowsx.h>
#define GLEW_STATIC
//#include <GL/gl.h>
//#include <wingl.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#define CLASS_NAME "Main Window Class"

static LARGE_INTEGER freq;

typedef struct Platform {
  HINSTANCE hInstance;
  HWND hWnd;

  DWORD consoleMode;

  HDC device;
  HDC bufDC;
  HBITMAP bufBits;
  HGLRC context;
  //WINGLContext context;

  uint16 width;
  uint16 height;

  bool mcapture;
  bool released;
  bool unfocused;

  bool fullscreen;
  bool fchanged;
  bool mstate;
  bool toggling;
  int32 wstyle;
  WINDOWPLACEMENT wplace;

} Platform;

static Platform platform;

static void disableCursor(void);
static void enableCursor(void);
static void centerCursor(void);
static void setClipRect(HWND hWnd);

static LRESULT CALLBACK procWinInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static uint8* buf;

bool platformInit(const char* appName, int32 x, int32 y, int32 width, int32 height) {

  platform.width = width;
  platform.height = height;
  SetConsoleTitleA(appName);

  platform.hInstance = GetModuleHandleA(0);

  WNDCLASSA wndClass = {};

  wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wndClass.lpfnWndProc = procWinInput;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = platform.hInstance;
  wndClass.hIcon = LoadIcon(platform.hInstance, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = CLASS_NAME;

  if (!RegisterClassA(&wndClass)) {
    MessageBoxA(NULL, "Failed to register window class", "Fatal error", MB_ICONERROR | MB_OK);
    return 0;
  }

  RECT wborder = {x, y, x+width, y+height};
  AdjustWindowRectEx(&wborder, WS_OVERLAPPEDWINDOW, 0, WS_EX_APPWINDOW);

  int32 wx = wborder.left, wy = wborder.top,
      ww = wborder.right - wborder.left, 
      wh = wborder.bottom - wborder.top;

  platform.hWnd = CreateWindowExA(
    WS_EX_APPWINDOW, CLASS_NAME, appName,
    WS_OVERLAPPEDWINDOW,
    wx, wy, ww, wh,
    NULL, NULL, platform.hInstance, NULL);

  if (platform.hWnd == NULL) {
    MessageBoxA(NULL, "Failed to create window", "Fatal error", MB_ICONERROR | MB_OK);
    return 0;
  }

  ShowWindow(platform.hWnd, SW_SHOW);
  UpdateWindow(platform.hWnd);

  QueryPerformanceFrequency(&freq);
  freq.QuadPart /= 1000000;

  GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &platform.consoleMode);
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR), 
    1,
    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA,
    32,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    24, 8,
    0, 0, 0, 0, 0, 0
  };

  platform.device = GetDC(platform.hWnd);
  uint32 pixelFormat = ChoosePixelFormat(platform.device, &pfd);

  if (!SetPixelFormat(platform.device, pixelFormat, &pfd)) {
    MessageBoxA(NULL, "Failed to set pixel format", "Fatal error", MB_ICONERROR | MB_OK);
    return 0;
  }

  platform.context = wglCreateContext(platform.device);
  //platform.context = winglCreateContext(platform.device);
  if (!platform.context) {
    MessageBoxA(NULL, "Failed to create OpenGL context", "Fatal error", MB_ICONERROR | MB_OK);
    return 0;
  }
  wglMakeCurrent(platform.device, platform.context);
  //winglMakeCurrent(platform.context);

  if (glewInit() != GLEW_OK  || wglewInit() != GLEW_OK) {
    MessageBoxA(NULL, "Failed to initialize GLAD", "Fatal error", MB_ICONERROR | MB_OK);
    return 0;
  }
  wglSwapIntervalEXT(0);
  
  glClearColor(0.1f, 0.1f, 0.1f, 1);
  
  return 1;
}

void platformShutdown() {
  wglMakeCurrent(platform.device, NULL);
  wglDeleteContext(platform.context);
  //winglDestroyContext(platform.context);

  ReleaseDC(platform.hWnd, platform.device);
  SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), platform.consoleMode);

  if (platform.hWnd) {
    DestroyWindow(platform.hWnd);
    platform.hWnd = 0;
  }

  UnregisterClassA("Snow Wnd Class", platform.hInstance);
}

void pollEvents() {
  MSG msg;
  while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }
}

void platformPresent() {
  wglSwapLayerBuffers(platform.device, WGL_SWAP_MAIN_PLANE);
  //winglSwapBuffers();
}

LRESULT procWinInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  int16 x, y, delta;
  ButtonCode button;
  bool down;
  POINT pos;
  bool outside;

  if (platform.fchanged && platform.mstate) {
    platformSetCapture(true);
    platform.fchanged = false;
  }

  switch (uMsg) {
    case WM_DESTROY:
      keOnQuit.fire();
      PostQuitMessage(0);
      return 0;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
      InputSystem::processKey((KeyCode)wParam, !(HIWORD(lParam) & KF_UP));
      return 0;
    
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
      GetCursorPos(&pos);
      ScreenToClient(hWnd, &pos);
      outside = (pos.x < 0 || pos.y < 0 || pos.x >= platform.width || pos.y >= platform.height);
      if (GetCapture() != hWnd && outside) return 0;

      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP) {
        button = BUTTON_LEFT;
      } else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP) {
        button = BUTTON_RIGHT;
      } else {
        button = BUTTON_MIDDLE;
      }

      down = uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN;
      
      if (outside && down) return 0;
      InputSystem::processButton(button, down);
      if (down) SetCapture(hWnd);

      if (GetCapture() == hWnd
      && InputSystem::isButtonUp(BUTTON_LEFT) 
      && InputSystem::isButtonUp(BUTTON_RIGHT) 
      && InputSystem::isButtonUp(BUTTON_MIDDLE)) {
        ReleaseCapture();
        platform.released = true;
      }
      return 0;

    case WM_MOUSEMOVE:
      
      if (GetForegroundWindow() != platform.hWnd) {
        if (!platform.unfocused && platform.mcapture) {
          InputSystem::processMouse(0, 0);
        }
        platform.unfocused = true;
        return 0;
      }
      if (platform.released) {
        platform.released = false;
        return 0;
      }
      
      if (platform.unfocused) {
        platform.unfocused = false;
        return 0;
      }

      x = GET_X_LPARAM(lParam);
      y = GET_Y_LPARAM(lParam);    

      if (x < 0 || y < 0 || x >= platform.width || y >= platform.height) return 0;

      if (platform.mcapture && x == platform.width/2 && y == platform.height/2) {
        x = 0;
        y = 0;
      } else if (platform.mcapture) {
        x -= platform.width/2;
        y -= platform.height/2;
        centerCursor();
      }

      InputSystem::processMouse(x, y);
      return 0;

    case WM_MOUSEWHEEL:
      delta = GET_WHEEL_DELTA_WPARAM(wParam);
      if (!delta) return 0;

      InputSystem::processWheel(delta > 0 ? 1 : -1);
      return 0;

    case WM_SIZE:
      if (platform.toggling) return 0;
      platform.width = LOWORD(lParam);
      platform.height = HIWORD(lParam);
      Renderer::onResize(platform.width, platform.height);
      keOnResize.fire(platform.width, platform.height);
      // static bool a = 0;
      // if (!a) {
      //   a = 1;
      //   return 0;
      // }
      //winglMakeCurrent(platform.context);
      return 0;
  }
  
  return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

uint64 getTime() {
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  return (double)ticks.QuadPart / freq.QuadPart;
}

void sleep(uint64 ms) {
  Sleep(ms);
}

void consoleWrite(const char* msg, ColorFlags color) {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

  WORD attr = (color & COLOR_RED ? FOREGROUND_RED : 0) 
  | (color & COLOR_GREEN ? FOREGROUND_GREEN : 0)
  | (color & COLOR_BLUE ? FOREGROUND_BLUE : 0)
  | (color & COLOR_BRIGHT ? FOREGROUND_INTENSITY : 0);

  SetConsoleTextAttribute(hOut, attr);

  WriteConsoleA(hOut, msg, strlen(msg), NULL, NULL);

  SetConsoleTextAttribute(hOut, 0x7);
}

void platformSetCapture(bool mode) {
  if (mode == platform.mcapture) return;
  if (mode) disableCursor();
  else enableCursor();
  platform.mcapture = mode;
}

void disableCursor(void) {
  ShowCursor(0);
  centerCursor();
  setClipRect(platform.hWnd);
}

void enableCursor(void) {
  ShowCursor(1);
  setClipRect(NULL);
}

void centerCursor(void) {
  POINT pt = {platform.width/2, platform.height/2};
  ClientToScreen(platform.hWnd, &pt);
  SetCursorPos(pt.x, pt.y);
}

void setClipRect(HWND wnd) {
  if (!wnd) {
    ClipCursor(NULL);
    return;
  }

  RECT r;
  GetClientRect(wnd, &r);
  ClientToScreen(wnd, (POINT*)&r.left);
  ClientToScreen(wnd, (POINT*)&r.right);
  ClipCursor(&r);

}

void platformToggleFullscreen() {
  platform.fullscreen = !platform.fullscreen;
  
  if (platform.fullscreen) {
    platform.wstyle = GetWindowLong(platform.hWnd, GWL_STYLE);
    GetWindowPlacement(platform.hWnd, &platform.wplace);

    platform.toggling = true;
    SetWindowLong(platform.hWnd, GWL_STYLE, WS_POPUP);
    platform.toggling = false;

    ShowWindow(platform.hWnd, SW_SHOWMAXIMIZED);
  } else {
    SetWindowLong(platform.hWnd, GWL_STYLE, platform.wstyle);
    SetWindowPlacement(platform.hWnd, &platform.wplace);
    ShowWindow(platform.hWnd, SW_SHOW);
  }

  platform.fchanged = true;
  platform.mstate = platform.mcapture;
  platformSetCapture(false);
  platform.unfocused = true;
}

#endif