#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>
#include <renderer/Model.h>
#include <renderer/Light.h>
#include GL_HEADER

static Vertex vertices[] = {
// 	  X      Y      Z     	 NX     NY     NZ      R     G     B     A       TX    TY
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},

  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},

  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},

  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
};

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static Model box;
static Model redBox;
static Model greenBox;
static Model blueBox;

static Light redLight;
static Light greenLight;
static Light blueLight;

static vec3 lightColor = vec3(1.0f, 0.5f, 0.31f);
static vec3 red = vec3(1.0f, 0.0f, 0.0f);
static vec3 green = vec3(0.0f, 1.0f, 0.0f);
static vec3 blue = vec3(0.0f, 0.0f, 1.0f);

static vec3 redPos = vec3(-.8f, .8f, .8f);
static vec3 greenPos = vec3(.8f, .8f, .8f);
static vec3 bluePos = vec3(0.f, -.8f, .8f);

static mat4 redModel = translate(scale(mat4(1.0f), vec3(0.05f)), redPos);
static mat4 greenModel = translate(scale(mat4(1.0f), vec3(0.05f)), greenPos);
static mat4 blueModel = translate(scale(mat4(1.0f), vec3(0.05f)), bluePos);

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);

  box = Model::create(vertices, sizeof(vertices)/sizeof(Vertex), "resources/textures/box256.jpg", SHADE_LIT);
  
  redBox = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  greenBox = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  blueBox = Model::create(vertices, sizeof(vertices)/sizeof(Vertex));
  
  redBox.setColor(red);
  greenBox.setColor(green);
  blueBox.setColor(blue);

  redLight.type = LIGHT_POINT;
  redLight.setColor(red);
  redLight.setPos(redPos);
  redLight.setFalloff(0.1f);
  redLight.enable();

  greenLight.type = LIGHT_POINT;
  greenLight.setColor(green);
  greenLight.setPos(greenPos);
  greenLight.setFalloff(0.1f);
  greenLight.enable();

  blueLight.type = LIGHT_POINT;
  blueLight.setColor(blue);
  blueLight.setPos(bluePos);
  blueLight.setFalloff(0.1f);
  blueLight.enable();
}

static mat4 model;
void GameManager::update() {
	if (InputSystem::wasKeyDown(KEY_SPACE) != InputSystem::isKeyDown(KEY_SPACE)) {
		KE_INFO("Space key is now %s", InputSystem::isKeyDown(KEY_SPACE) ? "down" : "up");
	}
  model = translate(
		rotate(mat4(1.0f), -30.0f*Time->time, vec3(0, 0, 1.0f)),
		vec3(0.0f, 0.5f, 0.0f)*sinf(2*Time->time)
	);
}

void GameManager::render() {
  redLight.update();
  greenLight.update();
  blueLight.update();

  glMatrixMode(GL_MODELVIEW);
 
  glPushMatrix();
  glMultMatrixf(redModel.data());
  redBox.draw();
  glPopMatrix();

  glPushMatrix();
  glMultMatrixf(greenModel.data());
  greenBox.draw();
  glPopMatrix();

  glPushMatrix();
  glMultMatrixf(blueModel.data());
  blueBox.draw();
  glPopMatrix();
  
  glPushMatrix();
	glMultMatrixf(model.data());
	box.draw();
  glPopMatrix();
}

void GameManager::shutdown() {
  redLight.disable();
  greenLight.disable();
  blueLight.disable();
	Model::destroy(box);
  Model::destroy(redBox);
  Model::destroy(greenBox);
  Model::destroy(blueBox);
}

void keyCallback(uint8 key, bool down) {
	KE_INFO("Key 0x%02x is now %s", key, down ? "down" : "up");
}
void buttonCallback(uint8 button, bool down) {
	KE_INFO("Button 0x%02x is now %s", button, down ? "down" : "up");
}

void moveCallback(int16 x, int16 y) {
	//KE_INFO("Mouse is at (%d; %d)", x, y);
}

void wheelCallback(int8 delta) {
	KE_INFO("Mouse wheel rotated %s", delta > 0 ? "forward" : "backward");
}

void resizeCallback(uint16 w, uint16 h) {
	KE_INFO("Window dimensions: %ux%u", w, h);
}