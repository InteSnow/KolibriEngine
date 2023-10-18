#include "Events.h"

Event<> keOnQuit;
Event<uint8, bool> keOnKey;
Event<uint8, bool> keOnMouseButton;
Event<int16, int16> keOnMouseMove;
Event<int8> keOnMouseWheel;
Event<uint16, uint16> keOnResize;