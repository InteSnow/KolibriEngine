#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>
#include <renderer/Model.h>
#include <renderer/Light.h>
#include <core/Object.h>
#include <systems/GUI.h>
#include <systems/Fonts.h>
#include GL_HEADER
#include <vector>
#include <core/File.h>
#include "ColoredButton.h"

std::vector<Vertex> vertices = {};

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static SceneObject* model1;
static SceneObject* model2;
static SceneObject* model3;
static SceneObject* sun;
static GUIObject* button;

extern float fps;

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);

  model1 = SceneObject::create<Model>(
    Model::create("resources/models/cube.obj", SHADE_LIT)
  );
  model1->transform.scale(vec3(0.5f));

  model2 = SceneObject::create<Model>(
    Model::create("resources/models/medieval.obj", SHADE_LIT)
  );
  model2->transform.scale(vec3(0.1f));
  model2->transform.move(vec3(-0.1f, -0.5f, 0.2f));
  model2->makeInactive();

  model3 = SceneObject::create<Model>(
    Model::create("resources/models/plane.obj", SHADE_LIT)
  );
  model3->transform.scale(vec3(0.2f));
  model3->makeInactive();

  sun = SceneObject::create<Light>(Light());
  sun->get<Light>()->type = LIGHT_DIRECT;
  sun->get<Light>()->setColor(vec3(0.99f, 0.98f, 0.83f));
  sun->get<Light>()->setDir(vec3(-1.0f, -1.0f, -1.0f));

  button = GUIObject::create<ColoredButton>(
    ColoredButton(vec3(1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f))
  );
  button->rect = Rect(100, 150, 150, 50, 10);


  uint32 glyphs[160];
  for (uint32 c = 32; c < 128; c++) {
    glyphs[c-32] = c;
  }
  for (uint32 c = 0x410; c <= 0x44f; c++) {
    glyphs[c-0x410+96] = c;
  }
  Fonts::load("resources/fonts/times.ttf", 40, glyphs, 160);
  Fonts::load("resources/fonts/arial.ttf", 80, glyphs, 160);
}

void GameManager::update() {
	if (InputSystem::wasKeyDown(KEY_SPACE) != InputSystem::isKeyDown(KEY_SPACE)) {
		KE_INFO("Space key is now %s", InputSystem::isKeyDown(KEY_SPACE) ? "down" : "up");
	}
  model1->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));
  model2->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));
  model3->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));
}

void GameManager::render() {
  Fonts::select("Arial", 80);
  Fonts::print(120, 100, 40, vec3(1.0f, 0.0f, 0.0f), "%d", (int)fps);
  Fonts::select("Times New Roman", 40);
  Fonts::print(20, 100, 40, vec3(1.0f), "FPS: ");
}

void GameManager::shutdown() {

}

void keyCallback(uint8 key, bool down) {
	KE_DEBUG("Key 0x%02x is now %s", key, down ? "down" : "up");
  if (key == KEY_1 && down) {
    model1->makeActive();
    model2->makeInactive();
    model3->makeInactive();
    model1->transform.position = vec3(0);
  }
  if (key == KEY_2 && down) {
    model1->makeInactive();
    model2->makeActive();
    model3->makeInactive();
  }
  if (key == KEY_3 && down) {
    model1->makeInactive();
    model2->makeInactive();
    model3->makeActive();
    model3->transform.position = vec3(0);
  }
  if (key == KEY_4 && down) {
    model1->makeActive();
    model2->makeActive();
    model3->makeActive();
    model1->transform.position = vec3(-2.0f, 0, 0);
    model3->transform.position = vec3(2.0f, 0, 0);
  }
}
void buttonCallback(uint8 button, bool down) {
	KE_DEBUG("Button 0x%02x is now %s", button, down ? "down" : "up");
}

void moveCallback(int16 x, int16 y) {
	//KE_INFO("Mouse is at (%d; %d)", x, y);
}

void wheelCallback(int8 delta) {
	KE_DEBUG("Mouse wheel rotated %s", delta > 0 ? "forward" : "backward");
}

void resizeCallback(uint16 w, uint16 h) {
	KE_DEBUG("Window dimensions: %ux%u", w, h);
}