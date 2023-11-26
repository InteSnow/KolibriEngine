#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>
#include <renderer/Model.h>
#include GL_HEADER

static Vertex vertices[] = {
// 	  X      Y      Z     	 NX     NY     NZ      R     G     B     A       TX    TY
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f, 1.0f, 1.f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.f}, {1.0f, 0.0f}},

  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {0.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f, 0.0f, 1.f}, {0.0f, 1.0f}},

  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f, 1.0f, 1.f}, {1.0f, 0.0f}},

  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {0.0f, 1.0f}},
  {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {1.0f, 0.0f}},
  {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f, 0.0f, 1.f}, {0.0f, 1.0f}},

  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {0.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {1.0f, 1.0f}},
  {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {1.0f, 0.0f}},
  {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {1.0f, 1.0f}},
  {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {0.0f, 0.0f}},
  {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f, 0.0f, 1.f}, {0.0f, 1.0f}},
};

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static Texture boxDiff;
static Texture boxSpec;

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);

	boxDiff = Texture::create("resources/textures/box256.jpg");
  boxSpec = Texture::create("resources/textures/box256_spec.jpg");
}

static mat4 model;
static mat4 lModel = translate(scale(mat4(1.0f), vec3(0.05f)), vec3(-.6f, .6f, 1.0f));
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
  glMatrixMode(GL_MODELVIEW);

  glDisable(GL_TEXTURE_2D);

  
  vec4 lightPos = vec4(-.6f, .6f, 1.0f, 1.0f);
  vec4 diffuse = vec4(1.0f, 0.5f, 0.31f, 1.0f);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, &diffuse.x);
  glLightfv(GL_LIGHT0, GL_POSITION, &lightPos.x);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.4f);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, .8f);

  glPushMatrix();
  glMultMatrixf(lModel.data());
  
  glDisable(GL_LIGHTING);
  draw(vertices, sizeof(vertices)/sizeof(Vertex));
  
  glPopMatrix();


  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  
  
  glPushMatrix();
	glMultMatrixf(model.data());
  
  

	boxDiff.bind(0);
  boxSpec.bind(1);
	draw(vertices, sizeof(vertices)/sizeof(Vertex));
  boxSpec.unbind();
  boxDiff.unbind();

  glPopMatrix();

  glDisable(GL_LIGHT0);
  
}

void GameManager::shutdown() {
	Texture::destroy(boxDiff);
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