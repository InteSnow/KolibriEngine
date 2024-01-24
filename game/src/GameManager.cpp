#include "GameManager.h"

#include <core/logger.h>
#include <core/File.h>
#include <core/Events.h>
#include <core/Object.h>

#include <renderer/renderer.h>
#include <renderer/Model.h>
#include <renderer/Light.h>

#include <physics/RigidBody.h>

#include <systems/InputSystem.h>
#include <systems/GUI.h>
#include <systems/Fonts.h>
#include <systems/Physics.h>

#include <vector>

#include "NamedSlider.h"

void keyCallback(uint8 key, bool down);
void buttonCallback(uint8 button, bool down);
void moveCallback(int16 x, int16 y);
void wheelCallback(int8 delta);
void resizeCallback(uint16 w, uint16 h);

static SceneObject* model1;
static SceneObject* model2;
static SceneObject* model3;
static SceneObject* sun;
static GUIObject* restitution;
static GUIObject* gravity;
static GUIObject* friction;
static GUIObject* fpsCounter;

static SceneObject* body1;
static SceneObject* body2;
static SceneObject* plane;

static std::vector<Vertex> vertices = {
  {{-1.0f, 0.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},
  {{ 1.0f, 0.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},
  {{ 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},

  {{-1.0f, 0.0f,-1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},
  {{ 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},
  {{-1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.83f,0.41f,0.26f}, {0.0f, 0.0f}},
};

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

  body1 = SceneObject::create<Model>(
    Model::create("resources/models/cube.obj", SHADE_LIT)
  );
  body1->transform.scale(vec3(0.25f));
  body1->add<RigidBody>(RigidBody(body1->get<Model>()->getBoundingBox()));
  body1->get<RigidBody>()->mass = 0.125f;
  body1->makeInactive();

  body2 = SceneObject::create<Model>(
    Model::create("resources/models/cube.obj", SHADE_LIT)
  );
  body2->transform.scale(vec3(0.5f));
  body2->add<RigidBody>(RigidBody(body2->get<Model>()->getBoundingBox()));
  body2->makeInactive();

  plane = SceneObject::create<Mesh>(
    Mesh::create(std::move(vertices), SHADE_LIT)
  );
  plane->add<RigidBody>(RigidBody(plane->get<Mesh>()->getBoundingBox()));
  plane->get<RigidBody>()->immovable = 1;
  plane->transform.move(vec3(0.0f, -1.0f, 0.0f));
  plane->transform.scale(vec3(10.0f));
  plane->makeInactive();

  uint32 glyphs[160];
  for (uint32 c = 32; c < 128; c++) {
    glyphs[c-32] = c;
  }
  for (uint32 c = 0x410; c <= 0x44f; c++) {
    glyphs[c-0x410+96] = c;
  }
  Fonts::load("resources/fonts/arial.ttf", 30, glyphs, 160);

  restitution = GUIObject::create<NamedSlider>(
    NamedSlider()
  );

  restitution->makeInactive();
  restitution->rect = Rect(25, 200, 150, 20, 0, vec4(0.0f));
  restitution->get<NamedSlider>()->setFont("Arial", 30, 30);
  restitution->get<NamedSlider>()->nameOffset = vec2(0.0f, 40.0f);
  restitution->get<NamedSlider>()->setName(L"Упругость");
  restitution->get<NamedSlider>()->setRange(vec2(0.0f, 1.0f));
  restitution->get<NamedSlider>()->setValue(0.25f);
  restitution->get<NamedSlider>()->setColor(vec3(0.7f));

  gravity = GUIObject::create<NamedSlider>(
    NamedSlider()
  );
  gravity->makeInactive();
  gravity->rect = Rect(25, 125, 150, 20, 0, vec4(0.0f));
  gravity->get<NamedSlider>()->setFont("Arial", 30, 30);
  gravity->get<NamedSlider>()->nameOffset = vec2(0.0f, 40.0f);
  gravity->get<NamedSlider>()->setName(L"g");
  gravity->get<NamedSlider>()->setRange(vec2(-20.0f, 0.0f));
  gravity->get<NamedSlider>()->setValue(-9.8f);
  gravity->get<NamedSlider>()->setColor(vec3(0.7f));

  friction = GUIObject::create<NamedSlider>(
    NamedSlider()
  );
  friction->makeInactive();
  friction->rect = Rect(25, 50, 150, 20, 0, vec4(0.0f));
  friction->get<NamedSlider>()->setFont("Arial", 30, 30);
  friction->get<NamedSlider>()->nameOffset = vec2(0.0f, 40.0f);
  friction->get<NamedSlider>()->setName(L"Трение");
  friction->get<NamedSlider>()->setRange(vec2(0.0f, 1.0f));
  friction->get<NamedSlider>()->setValue(1.0f);
  friction->get<NamedSlider>()->setColor(vec3(0.7f));

  fpsCounter = GUIObject::create<>();
  fpsCounter->rect = Rect(Renderer::getFrameSize().x-150, Renderer::getFrameSize().y-50, 100, 40, 0, vec4(0.0f));
  fpsCounter->text = Text(&fpsCounter->rect, "Arial", 30, 30, vec3(1.0f));
  fpsCounter->text.center = 0;
}

void GameManager::update() {
	if (InputSystem::wasKeyDown(KEY_SPACE) != InputSystem::isKeyDown(KEY_SPACE)) {
		KE_INFO("Space key is now %s", InputSystem::isKeyDown(KEY_SPACE) ? "down" : "up");
	}
  model1->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));
  model2->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));
  model3->transform.rotate(-30.0f*Time->deltaTime*vec3(0, 1.0f, 0));

  fpsCounter->text.setText(L"FPS: %d", (int)fps);
  body1->get<RigidBody>()->restitution = restitution->get<NamedSlider>()->getValue();
  body2->get<RigidBody>()->restitution = restitution->get<NamedSlider>()->getValue();
  plane->get<RigidBody>()->restitution = restitution->get<NamedSlider>()->getValue();
  Physics::g = gravity->get<NamedSlider>()->getValue();
  body1->get<RigidBody>()->friction = friction->get<NamedSlider>()->getValue();
  body2->get<RigidBody>()->friction = friction->get<NamedSlider>()->getValue();
  plane->get<RigidBody>()->friction = friction->get<NamedSlider>()->getValue();
}

void GameManager::render() {
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
    body1->makeInactive();
    body2->makeInactive();
    plane->makeInactive();
    restitution->makeInactive();
    gravity->makeInactive();
    friction->makeInactive();
  }
  if (key == KEY_2 && down) {
    model1->makeInactive();
    model2->makeActive();
    model3->makeInactive();
    body1->makeInactive();
    body2->makeInactive();
    plane->makeInactive();
    restitution->makeInactive();
    gravity->makeInactive();
    friction->makeInactive();
  }
  if (key == KEY_3 && down) {
    model1->makeInactive();
    model2->makeInactive();
    model3->makeActive();
    model3->transform.position = vec3(0);
    body1->makeInactive();
    body2->makeInactive();
    plane->makeInactive();
    restitution->makeInactive();
    gravity->makeInactive();
    friction->makeInactive();
  }
  if (key == KEY_4 && down) {
    model1->makeActive();
    model2->makeActive();
    model3->makeActive();
    model1->transform.position = vec3(-2.0f, 0, 0);
    model3->transform.position = vec3(2.0f, 0, 0);
    body1->makeInactive();
    body2->makeInactive();
    plane->makeInactive();
    restitution->makeInactive();
    gravity->makeInactive();
    friction->makeInactive();
  }
  if (key == KEY_5 && down) {
    model1->makeInactive();
    model2->makeInactive();
    model3->makeInactive();
    body1->transform.position = vec3(-1.0f, -0.5f, 0.0f);
    body1->get<RigidBody>()->setImpulse(vec3(0.5f, 1.0f, 0.0f));
    body2->transform.position = vec3(1.0f, 2.5f, 0.0f);
    body2->get<RigidBody>()->setImpulse(vec3(0.0f, 0.0f, 0.0f));
    body1->makeActive();
    body2->makeActive();
    plane->makeActive();
    restitution->makeActive();
    gravity->makeActive();
    friction->makeActive();
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
  fpsCounter->rect = Rect(Renderer::getFrameSize().x-150, Renderer::getFrameSize().y-50, 100, 40, 0, vec4(0.0f));
}