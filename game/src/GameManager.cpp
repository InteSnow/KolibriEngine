#include "GameManager.h"
#include <core/logger.h>
#include <systems/InputSystem.h>
#include <core/Events.h>
#include <renderer/Model.h>
#include <renderer/Light.h>
#include <core/GameObject.h>
#include GL_HEADER
#include <GL/glu.h>
#include <vector>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <core/File.h>

std::vector<Vertex> vertices = {};

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static GameObject* model1;
static GameObject* model2;
static GameObject* model3;
static GameObject* sun;

static Texture bitmapTex;
static stbtt_bakedchar cdata[96];

void GameManager::init() {
	keOnKey.subscribe(keyCallback);
	keOnMouseButton.subscribe(buttonCallback);
	keOnMouseMove.subscribe(moveCallback);
	keOnMouseWheel.subscribe(wheelCallback);
	keOnResize.subscribe(resizeCallback);

  model1 = GameObject::create<Model>();
  *model1->get<Model>() = Model::create("resources/models/cube.obj", SHADE_LIT);
  model1->transform.scale(vec3(0.5f));

  model2 = GameObject::create<Model>();
  *model2->get<Model>() = Model::create("resources/models/medieval.obj", SHADE_LIT);
  model2->transform.scale(vec3(0.1f));
  model2->transform.move(vec3(-0.1f, -0.5f, 0.2f));
  model2->makeInactive();

  model3 = GameObject::create<Model>();
  *model3->get<Model>() = Model::create("resources/models/plane.obj", SHADE_LIT);
  model3->transform.scale(vec3(0.2f));
  model3->makeInactive();

  sun = GameObject::create<Light>();
  sun->get<Light>()->type = LIGHT_DIRECT;
  sun->get<Light>()->setColor(vec3(0.99f, 0.98f, 0.83f));
  sun->get<Light>()->setDir(vec3(-1.0f, -1.0f, -1.0f));

  std::string fontBuffer;

  File font = File::open("resources/fonts/arial.ttf", FILE_READ | FILE_BINARY);
  font.readAll(fontBuffer);
  File::close(font);
  stbtt_fontinfo fontinfo;

  int b_w = 256, b_h = 256, l_h = 32;
  uint8* bitmap = (uint8*)malloc(b_w*b_h);
  stbtt_BakeFontBitmap((const uint8*)fontBuffer.c_str(), 0, l_h, bitmap, b_w, b_h, 32, 96, cdata);

  bitmapTex = Texture::create(bitmap, b_w, b_h, 1);
  free(bitmap);
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
  bitmapTex.bind(0);
  glBegin(GL_QUADS);

  float x = 100, y = 100;
  stbtt_aligned_quad q;
  stbtt_GetBakedQuad(cdata, 256, 256, 'A'-32, &x, &y, &q, 1);
  glTexCoord2f(q.s0, q.t0); glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(q.x0, q.y0);
  glTexCoord2f(q.s1, q.t0); glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(q.x1, q.y0);
  glTexCoord2f(q.s1, q.t1); glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(q.x1, q.y1);
  glTexCoord2f(q.s0, q.t1); glColor3f(1.0f, 1.0f, 1.0f); glVertex2f(q.x0, q.y1);

  glEnd();
  bitmapTex.unbind();
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