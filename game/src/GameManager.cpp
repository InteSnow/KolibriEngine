#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>
#include <renderer/Model.h>
#include <renderer/Light.h>
#include <core/GameObject.h>
#include GL_HEADER

static Vertex vertices[] = {
// 	  X      Y      Z     	 NX     NY     NZ      R     G     B        TX    TY
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},

  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},

  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
};

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static GameObject box;
static GameObject redBox;
static GameObject greenBox;
static GameObject blueBox;

static vec3 lightColor = vec3(1.0f, 0.5f, 0.31f);
static vec3 red = vec3(1.0f, 0.0f, 0.0f);
static vec3 green = vec3(0.0f, 1.0f, 0.0f);
static vec3 blue = vec3(0.0f, 0.0f, 1.0f);

static vec3 redPos = vec3(-.8f, .8f, .8f);
static vec3 greenPos = vec3(.8f, .8f, .8f);
static vec3 bluePos = vec3(0.f, -.8f, .8f);

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);

  box = GameObject::create<Model>();
  *box.get<Model>() = Model::create(vertices, sizeof(vertices)/sizeof(Vertex), "resources/textures/box256.jpg", SHADE_LIT);

  redBox = GameObject::create<Model, Light>();
  *redBox.get<Model>() = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  redBox.get<Model>()->setColor(red);
  
  greenBox = GameObject::create<Model, Light>();
  *greenBox.get<Model>() = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  greenBox.get<Model>()->setColor(green);
  
  blueBox = GameObject::create<Model, Light>();
  *blueBox.get<Model>() = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  blueBox.get<Model>()->setColor(blue);

  redBox.get<Light>()->type = LIGHT_POINT;
  redBox.get<Light>()->setColor(red);
  redBox.get<Light>()->setFalloff(0.1f);

  greenBox.get<Light>()->type = LIGHT_POINT;
  greenBox.get<Light>()->setColor(green);
  greenBox.get<Light>()->setFalloff(0.1f);

  blueBox.get<Light>()->type = LIGHT_POINT;
  blueBox.get<Light>()->setColor(blue);
  blueBox.get<Light>()->setFalloff(0.1f);

  redBox.transform.position = redPos;
  redBox.transform.size = vec3(0.05f);
  greenBox.transform.position = greenPos;
  greenBox.transform.size = vec3(0.05f);
  blueBox.transform.position = bluePos;
  blueBox.transform.size = vec3(0.05f);
}

void GameManager::update() {
	if (InputSystem::wasKeyDown(KEY_SPACE) != InputSystem::isKeyDown(KEY_SPACE)) {
		KE_INFO("Space key is now %s", InputSystem::isKeyDown(KEY_SPACE) ? "down" : "up");
	}
  box.transform.rotation = -30.0f*Time->time*vec3(0, 0, 1.0f);
  box.transform.position = vec3(0, 0.5f, 0)*sinf(2*Time->time);
}

void GameManager::render() {
  redBox.onRenderBegin();
  greenBox.onRenderBegin();
  blueBox.onRenderBegin();

  redBox.onDraw();
  greenBox.onDraw();
  blueBox.onDraw();  

  box.onDraw();
}

void GameManager::shutdown() {
  GameObject::destroy(redBox);
  GameObject::destroy(greenBox);
  GameObject::destroy(blueBox);
  GameObject::destroy(box);
}

void keyCallback(uint8 key, bool down) {
	KE_DEBUG("Key 0x%02x is now %s", key, down ? "down" : "up");
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