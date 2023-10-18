#pragma once
#include "defines.h"

typedef enum KeyCode {
  KEY_BACK      = 0x08,
  KEY_TAB       = 0x09, 
  KEY_CLEAR     = 0x0C,
  KEY_ENTER     = 0x0D,
  
  KEY_PAUSE     = 0x13,
  KEY_CAPSLOCK  = 0x14,
  
  KEY_ESCAPE    = 0x1B,
  KEY_SPACE     = 0x20,
  
  KEY_PGUP      = 0x21,
  KEY_PGDN      = 0x22,
  KEY_END       = 0x23,
  KEY_HOME      = 0x24,
  
  // Arrow keys
  KEY_LEFT      = 0x25,
  KEY_UP        = 0x26,
  KEY_RIGHT     = 0x27,
  KEY_DOWN      = 0x28,
  
  KEY_PRINTSCR  = 0x2C,
  KEY_INS       = 0x2D,
  KEY_DEL       = 0x2E,

  KEY_0         = 0x30,
  KEY_1         = 0x31,
  KEY_2         = 0x32,
  KEY_3         = 0x33,
  KEY_4         = 0x34,
  KEY_5         = 0x35,
  KEY_6         = 0x36,
  KEY_7         = 0x37,
  KEY_8         = 0x38,
  KEY_9         = 0x39,

  KEY_A         = 0x41,
  KEY_B         = 0x42,
  KEY_C         = 0x43,
  KEY_D         = 0x44,
  KEY_E         = 0x45,
  KEY_F         = 0x46,
  KEY_G         = 0x47,
  KEY_H         = 0x48,
  KEY_I         = 0x49,
  KEY_J         = 0x4A,
  KEY_K         = 0x4B,
  KEY_L         = 0x4C,
  KEY_M         = 0x4D,
  KEY_N         = 0x4E,
  KEY_O         = 0x4F,
  KEY_P         = 0x50,
  KEY_Q         = 0x51,
  KEY_R         = 0x52,
  KEY_S         = 0x53,
  KEY_T         = 0x54,
  KEY_U         = 0x55,
  KEY_V         = 0x56,
  KEY_W         = 0x57,
  KEY_X         = 0x58,
  KEY_Y         = 0x59,
  KEY_Z         = 0x5A,

  KEY_LSUPER    = 0x5B,
  KEY_RSUPER    = 0x5C,
  KEY_APPS      = 0x5D,

  KEY_N0        = 0x60,
  KEY_N1        = 0x61,
  KEY_N2        = 0x62,
  KEY_N3        = 0x63,
  KEY_N4        = 0x64,
  KEY_N5        = 0x65,
  KEY_N6        = 0x66,
  KEY_N7        = 0x67,
  KEY_N8        = 0x68,
  KEY_N9        = 0x69,

  KEY_MULTIPLY  = 0x6A,
  KEY_ADD       = 0x6B,
  KEY_SUBTRACT  = 0x6D,
  KEY_DECIMAL   = 0x6E,
  KEY_DIVIDE    = 0x6F,

  KEY_F1        = 0x70,
  KEY_F2        = 0x71,
  KEY_F3        = 0x72,
  KEY_F4        = 0x73,
  KEY_F5        = 0x74,
  KEY_F6        = 0x75,
  KEY_F7        = 0x76,
  KEY_F8        = 0x77,
  KEY_F9        = 0x78,
  KEY_F10       = 0x79,
  KEY_F11       = 0x7A,
  KEY_F12       = 0x7B,
  KEY_F13       = 0x7C,
  KEY_F14       = 0x7D,
  KEY_F15       = 0x7E,

  KEY_NUMLOCK    = 0x90,
  KEY_SCRLOCK    = 0x91,

  KEY_LSHIFT    = 0xA0,
  KEY_RSHIFT    = 0xA1,
  KEY_LCTRL     = 0xA2,
  KEY_RCTRL     = 0xA3,
  KEY_LMENU     = 0xA4,
  KEY_RMENU     = 0xA5,

  KEY_COLON     = 0xBA,
  KEY_PLUS      = 0xBB,
  KEY_COMMA     = 0xBC,
  KEY_MINUS     = 0xBD,
  KEY_PERIOD    = 0xBE,
  KEY_FSLASH    = 0xBF,
  KEY_TILDE     = 0xC0,

  KEY_LBRACKET  = 0xDB,
  KEY_BACKSLASH = 0xDC,
  KEY_RBRACKET  = 0xDD,
  KEY_QUOTE     = 0xDE,

  _KEY_CODE_MAX_ENUM
} KeyCode;

typedef enum ButtonCode {
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_MIDDLE,

  _BUTTON_CODE_MAX_ENUM
} ButtonCode;

struct KeyboardState {
  bool keys[_KEY_CODE_MAX_ENUM];
};

struct MouseState {
  int16 x;
  int16 y;
  bool buttons[_BUTTON_CODE_MAX_ENUM];
};

class InputSystem {

  static KeyboardState pKeyboard;
  static KeyboardState cKeyboard;
  static MouseState pMouse;
  static MouseState cMouse;

public:
  static bool init(void);
  static void shutdown(void);
  static void update(void);

  static void processKey(KeyCode key, bool down);
  static void processButton(ButtonCode button, bool down);
  static void processMouse(int16 x, int16 y);
  static void processWheel(int8 delta);

  static bool wasKeyUp(KeyCode key);
  static bool wasKeyDown(KeyCode key); 
  static bool isKeyUp(KeyCode key); 
  static bool isKeyDown(KeyCode key); 

  static bool wasButtonUp(ButtonCode button);
  static bool wasButtonDown(ButtonCode button); 
  static bool isButtonUp(ButtonCode button); 
  static bool isButtonDown(ButtonCode button); 

  static void getMousePos(int16& x, int16& y);
  static void getPrevMousePos(int16& x, int16& y);
};