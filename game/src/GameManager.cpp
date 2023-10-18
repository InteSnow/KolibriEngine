#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);
}

void GameManager::update() {
	if (InputSystem::wasKeyDown(KEY_SPACE) != InputSystem::isKeyDown(KEY_SPACE)) {
		KE_INFO("Space key is now %s", InputSystem::isKeyDown(KEY_SPACE) ? "down" : "up");
	}
}

void GameManager::shutdown() {

}

void keyCallback(uint8 key, bool down) {
	KE_INFO("Key 0x%02x is now %s", key, down ? "down" : "up");
}
void buttonCallback(uint8 button, bool down) {
	KE_INFO("Button 0x%02x is now %s", button, down ? "down" : "up");
}

void moveCallback(int16 x, int16 y) {
	KE_INFO("Mouse is at (%d; %d)", x, y);
}

void wheelCallback(int8 delta) {
	KE_INFO("Mouse wheel rotated %s", delta > 0 ? "forward" : "backward");
}

void resizeCallback(uint16 w, uint16 h) {
	KE_INFO("Window dimensions: %ux%u", w, h);
}