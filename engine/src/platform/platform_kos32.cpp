#include "platform.h"

#ifdef KE_PLATFORM_KOS32
#include "systems/InputSystem.h"
#include "core/Events.h"
#include <cstring>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <GL/osmesa.h>
#include <GL/gl.h>

struct BlitInfo {
  int32 dstx;
  int32 dsty;
  int32 w;
  int32 h;

  int32 srcx;
  int32 srcy;
  int32 srcw;
  int32 srch;

  void *bitmap;
  int32 stride;
};

static std::map<uint16, KeyCode> keys = {
  {0x1E, KEY_A},
  {0x30, KEY_B},
  {0x2E, KEY_C},
  {0x20, KEY_D},
  {0x12, KEY_E},
  {0x21, KEY_F},
  {0x22, KEY_G},
  {0x23, KEY_H},
  {0x17, KEY_I},
  {0x24, KEY_J},
  {0x25, KEY_K},
  {0x26, KEY_L},
  {0x32, KEY_M},
  {0x31, KEY_N},
  {0x18, KEY_O},
  {0x19, KEY_P},
  {0x10, KEY_Q},
  {0x13, KEY_R},
  {0x1F, KEY_S},
  {0x14, KEY_T},
  {0x16, KEY_U},
  {0x2F, KEY_V},
  {0x11, KEY_W},
  {0x2D, KEY_X},
  {0x16, KEY_Y},
  {0x2C, KEY_Z},

  {0x02, KEY_1},
  {0x03, KEY_2},
  {0x04, KEY_3},
  {0x05, KEY_4},
  {0x06, KEY_5},
  {0x07, KEY_6},
  {0x08, KEY_7},
  {0x09, KEY_8},
  {0x0A, KEY_9},
  {0x0B, KEY_0},
  {0x4F, KEY_N1},
  {0x50, KEY_N2},
  {0x51, KEY_N3},
  {0x4B, KEY_N4},
  {0x4C, KEY_N5},
  {0x4D, KEY_N6},
  {0x47, KEY_N7},
  {0x48, KEY_N8},
  {0x49, KEY_N9},
  {0x52, KEY_N0},

  {0x45, KEY_NUMLOCK},
  {0xE035, KEY_DIVIDE},
  {0x37, KEY_MULTIPLY},
  {0x4A, KEY_SUBTRACT},
  {0x4E, KEY_ADD},
  {0xE01C, KEY_ENTER},
  {0x53, KEY_DECIMAL},

  {0x29, KEY_TILDE},
  {0x0C, KEY_MINUS},
  {0x0D, KEY_PLUS},
  {0x2B, KEY_BACKSLASH},
  {0x1A, KEY_LBRACKET},
  {0x1B, KEY_RBRACKET},
  {0x27, KEY_COLON},
  {0x28, KEY_QUOTE},
  {0x33, KEY_COMMA},
  {0x34, KEY_PERIOD},
  {0x35, KEY_FSLASH},

  {0x46, KEY_SCRLOCK},
  {0xE052, KEY_INS},
  {0xE047, KEY_HOME},
  {0xE04F, KEY_END},
  {0xE049, KEY_PGUP},
  {0xE051, KEY_PGDN},
  {0xE053, KEY_DEL},

  {0xE048, KEY_UP},
  {0xE04B, KEY_LEFT},
  {0xE050, KEY_DOWN},
  {0xE04D, KEY_RIGHT},

  {0x1C, KEY_ENTER},
  {0x01, KEY_ESCAPE},
  {0x0E, KEY_BACK},
  {0x39, KEY_SPACE},
  {0x0F, KEY_TAB},
  {0x3A, KEY_CAPSLOCK},

  {0x2A, KEY_LSHIFT},
  {0x1D, KEY_LCTRL},
  {0x38, KEY_LMENU},
  {0xE05B, KEY_LSUPER},
  {0x36, KEY_RSHIFT},
  {0xE01D, KEY_RCTRL},
  {0xE038, KEY_RMENU},
  {0xE05C, KEY_RSUPER},
  {0xE05D, KEY_APPS},

  {0x3B, KEY_F1},
  {0x3C, KEY_F2},
  {0x3D, KEY_F3},
  {0x3E, KEY_F4},
  {0x3F, KEY_F5},
  {0x40, KEY_F6},
  {0x41, KEY_F7},
  {0x42, KEY_F8},
  {0x43, KEY_F9},
  {0x44, KEY_F10},
  {0x57, KEY_F11},
  {0x58, KEY_F12},

  {0xE145, KEY_PAUSE},
  {0xE037, KEY_PRINTSCR}
};

static uint32 getFreq(void);

static void loadConsole(void);
static void* loadFun(void* table, const char* name);
static void __stdcall (*con_set_title)(const char* title) = NULL;
static void __stdcall (*con_exit)(bool closeWindow) = NULL;
static void __stdcall (*con_write_asciiz)(const char* str) = NULL;
static uint32 __stdcall (*con_set_flags)(uint32 newFlags) = NULL;

static void beginDraw(void);
static void endDraw(void);
static void createWindow(uint16 x, uint16 y, uint16 width, uint16 height, const char* title);

static void getWindowSize(uint16& width, uint16& height);
static uint16 getSkinHeight(void);

static void setInputMode(bool useScancodes);
static uint8 getKey(void);

static uint32 getMouseEvents(void);
static void getMousePos(int16& x, int16& y);
static int16 getWheelDelta(void);

static void setEventMask(uint32 mask);
static uint32 getButtonId(void);
static uint16 getEvent(void);

static void blitWindow(void *bitmap, int32 dst_x, int32 dst_y,
                      int32 src_x, int32 src_y, int32 w, int32 h,
                      int32 src_w, int32 src_h, int32 stride);

#define BORDER 5

enum PlatformEvents {
  EVENT_REDRAW = 1,
  EVENT_KEY = 2,
  EVENT_WBUTTON = 3,
  EVENT_MOUSE = 6
};

struct Platform {
  uint32 freq;
  uint16 skinh;
  uint16 width;
  uint16 height;
  
  OSMesaContext context;
  void* buffer; 
};

static Platform platform;

bool platformInit(const char* appName, int32 x, int32 y, int32 width, int32 height) {
  loadConsole();
  if (!con_exit) {
    KE_ERROR("Failed to load console library");
    return false;
  }

  con_set_title(appName);

  platform.freq = getFreq();
  platform.skinh = getSkinHeight();
  
  beginDraw();
  createWindow(x, y, width+2*BORDER, height+platform.skinh+BORDER-1, appName);
  endDraw();

  platform.width = width;
  platform.height = height;

  setInputMode(1);

  setEventMask(0b10000000000000000000000000100111);
  platform.context = OSMesaCreateContextExt(OSMESA_BGRA, 0, 0, 0, NULL);
  if (!platform.context) {
    KE_ERROR("Failed to create Mesa context");
    return false;
  }

  platform.buffer = malloc(width*height*4*sizeof(GLubyte));
  if (!OSMesaMakeCurrent(platform.context, platform.buffer, GL_UNSIGNED_BYTE, width, height)) {
    KE_ERROR("Failed to set current context");
  }
  OSMesaPixelStore(OSMESA_Y_UP, 0);
  
  glClearColor(0.1f, 0.1f, 0.1f, 1);
  glViewport(0, 0, width, height);
  glScalef(1, -1, 1);
  return true;
}

void platformShutdown() {
  con_exit(true);
}

void pollEvents() {
  uint16 e;
  uint16 scancode = 0;
  uint16 keycode;
  bool down;
  static uint8 extcode = 0;

  uint16 width, height;

  uint32 mEvents;
  uint8 button;
  static int16 x = -1, y = -1;
  int16 ox, oy;
  while ((e = getEvent()) != 0) {
    KeyCode key;
    switch (e) {
    case EVENT_REDRAW:
      beginDraw();
      createWindow(0, 0, 0, 0, NULL);
      endDraw();

      getWindowSize(width, height);
      width -= 2*BORDER - 1;
      height -= platform.skinh + BORDER - 2;
      if (width != platform.width || height != platform.height) {
        platform.buffer = malloc(width*height*4*sizeof(GLubyte));
        memset(platform.buffer, 0, width*height*4*sizeof(GLubyte));
        OSMesaMakeCurrent(platform.context, platform.buffer, GL_UNSIGNED_BYTE, width, height);
        glViewport(0, 0, width, height);
        keOnResize.fire(width, height);
        platform.width = width;
        platform.height = height;
      }
      break;
    case EVENT_KEY:
      scancode = getKey();

      if (scancode == 0xE0 || scancode == 0xE1) {
        extcode = scancode;
        break;
      }
      if (extcode == 0xE1 && (scancode & 0x7F) == 0x1D) {
        break;
      }

      down = ~scancode & 0x80;
      scancode &= 0x7F;

      keycode = extcode << 8 | scancode;

      if (key = keys[keycode]) {
        InputSystem::processKey(key, down);
      }

      extcode = 0;

      break;
    case EVENT_WBUTTON:
      if (getButtonId() == 1) {
        keOnQuit.fire();
      }
      break;
    case EVENT_MOUSE:
      mEvents = getMouseEvents();
      ox = x;
      oy = y;

      getMousePos(x, y);
      x -= BORDER;
      y -= platform.skinh;
      if (x < 0 || y < 0 || x >= platform.width || y >= platform.height) {
        InputSystem::processButton(BUTTON_LEFT, 0);
        InputSystem::processButton(BUTTON_RIGHT, 0);
        InputSystem::processButton(BUTTON_MIDDLE, 0);
        break;
      }
      if (!mEvents && (x != ox || y != oy)) {
        InputSystem::processMouse(x, y);
      } else if (mEvents & 0b000001000000000000000) {
        InputSystem::processWheel(getWheelDelta() > 0 ? -1 : 1);
      } else if (mEvents & 0b111110001111100000000) {
        down = mEvents & 0b1111100000000;
        button = (down ? mEvents >> 8 : mEvents >> 16) & 0b111;
        button = button & 0b1 ? BUTTON_LEFT : (button & 0b10 ? BUTTON_RIGHT : BUTTON_MIDDLE);
        InputSystem::processButton((ButtonCode)button, down);
      }
      break;
    }
  }
}

void platformPresent() {
  blitWindow(platform.buffer,
    BORDER, platform.skinh, 0, 0,
    platform.width, platform.height, platform.width, platform.height, platform.width*4);
} 

uint64 getTime() {
   
  uint64 val;
  asm volatile(
  "cpuid"
  );
  asm volatile(
  "rdtsc"
  :"=A"(val));

  return (float64)val/platform.freq*1000000;
}

uint32 getFreq() {
  uint32 freq;
  asm volatile (
    "int $0x40"
    : "=a" (freq)
    : "a" (18), "b" (5)
  );
  return freq;
}

void consoleWrite(const char* msg, ColorFlags color) {
  uint32 flags = (color & COLOR_RED ? 0x04 : 0) | (color & COLOR_GREEN ? 0x02 : 0)
    | (color & COLOR_BLUE ? 0x01 : 0) | (color & COLOR_BRIGHT ? 0x08 : 0);

  uint32 oldFlags = con_set_flags(flags);
  con_write_asciiz(msg);
  con_set_flags(oldFlags);
}

void loadConsole() {
  const char* path = "/sys/lib/console.obj";

  void* table;
  asm volatile(
    "int $0x40"
    : "=a" (table)
    : "a" (68), "b" (19), "c" (path)
    : "memory"
  );

  con_set_title = (void __stdcall(*)(const char*))loadFun(table, "con_set_title");
  con_exit = (void __stdcall(*)(bool))loadFun(table, "con_exit");
  con_write_asciiz = (void __stdcall(*)(const char*))loadFun(table, "con_write_asciiz");
  con_set_flags = (uint32 __stdcall(*)(uint32))loadFun(table, "con_set_flags");
}

void* loadFun(void* table, const char* name) {
  if (!table) return NULL;

  while (*(uint32*)table) {
    if (!strcmp(*(const char**)table, name)) {
      void* ptr = *(void**)(table + 4);
      return ptr;
    }

    table += 8;
  }

  return NULL;
}

void beginDraw() {
  asm volatile ("int $0x40" :: "a" (12), "b" (1));
}

void endDraw() {
  asm volatile ("int $0x40" :: "a" (12), "b" (2));
}

void createWindow(uint16 x, uint16 y, uint16 width, uint16 height, const char* title) {
  asm volatile (
    "int $0x40"
    ::"a" (0),
      "b" ((x << 16) | (width-1)),
      "c" ((y << 16) | (height-1)),
      "d" ((1 << 28) | (3 << 24)),
      "D" (title),
      "S" (0)
    : "memory"
  );
}

void getWindowSize(uint16& width, uint16& height) {
  char buf[1024];
  asm volatile (
    "int $0x40"
    :: "a" (9), "b" (buf), "c" (-1)
  );
  width = *(uint16*)(buf + 42);
  height = *(uint16*)(buf + 46);
}

uint16 getSkinHeight(void) {
  uint32 skinh;
  asm volatile (
    "int $0x40"
    : "=a" (skinh)
    : "a" (48), "b" (4)
  );
  return skinh;
}

void setInputMode(bool useScancodes) {
  asm volatile (
    "int $0x40"
    :: "a" (66), "b" (1), "c" (useScancodes)
  );
}

uint8 getKey() {
  uint32 key;
  asm volatile (
    "int $0x40"
    : "=a" (key)
    : "a" (2)
  );

  return (key >> 8) & 0xFF;
}

uint32 getMouseEvents() {
  uint32 events;
  asm volatile (
    "int $0x40"
    : "=a" (events)
    : "a" (37), "b" (3)
  );
  return events;
}

void getMousePos(int16& x, int16& y) {
  uint32 output;
  asm volatile (
    "int $0x40"
    : "=a" (output)
    : "a" (37), "b" (1)
  );
  x = (output >> 16) & 0xFFFF;
  y = output & 0xFFFF;
}

int16 getWheelDelta(void) {
  int32 delta;
  asm volatile (
    "int $0x40"
    : "=a" (delta)
    : "a" (37), "b" (7)
  );
  return delta & 0xFFFF;
}

void setEventMask(uint32 mask) {
  asm volatile (
    "int $0x40"
    :: "a" (40), "b" (mask)
  );
}

uint16 getEvent() {
  uint16 e;
  asm volatile (
    "int $0x40"
    : "=a" (e)
    : "a" (11)
  );
  return e;
}

uint32 getButtonId() {
  uint32 id;
  asm volatile (
    "int $0x40"
    : "=a" (id)
    : "a" (17)
  );
  return id >> 8;
}

void blitWindow(void *bitmap, int32 dst_x, int32 dst_y,
                int32 src_x, int32 src_y, int32 w, int32 h,
                int32 src_w, int32 src_h, int32 stride) {
    volatile BlitInfo bc;

    bc.dstx = dst_x;
    bc.dsty = dst_y;
    bc.w    = w;
    bc.h    = h;
    bc.srcx = src_x;
    bc.srcy = src_y;
    bc.srcw = src_w;
    bc.srch = src_h;
    bc.stride = stride;
    bc.bitmap = bitmap;

    asm volatile(
    "int $0x40"
    :: "a"(73), "b"(0), "c"(&bc.dstx));
};

#endif