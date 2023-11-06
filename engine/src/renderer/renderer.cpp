#include "renderer.h"
#include "core/logger.h"
#include "utils/sml.hpp"
#include "core/Events.h"
#include "renderer/Camera.h"
#include "systems/InputSystem.h"
#include GL_HEADER

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef KE_PLATFORM_WIN32
#define HIRES_TEXTURES 1
#else
#define HIRES_TEXTURES 0
#endif

// static float32 vertices[] = {
//   //  X      Y      Z     R     G     B
//   -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
//    0.0f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
//    0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f
// };

static float32 vertices[] = {
  //  X      Y      Z     R     G     B    TX    TY
  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
   0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

   0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
   0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
  -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

   0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
   0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
   0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
   0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

  -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
   0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
   0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
   0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
};

static uint32 VAO;
static uint32 VBO;

static mat4 model;
static mat4 view;
static mat4 projection;

uint16 Renderer::frameWidth;
uint16 Renderer::frameHeight;
Shader Renderer::shader;
Camera* Renderer::camera;

uint32 boxTex;

bool Renderer::init(uint16 width, uint16 height) {
  Renderer::frameWidth = width;
  Renderer::frameHeight = height;
  Renderer::shader = Shader("resources/shaders/vertex.vert", "resources/shaders/fragment.frag");
  
  keOnResize.subscribe(Renderer::onResize);
  keOnKey.subscribe(Renderer::onKey);

  glClearColor(0.1f, 0.1f, 0.1f, 1);
  
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);
  
  KE_INFO("%s", glGetString(GL_VERSION));
  KE_INFO("%s", glGetString(GL_RENDERER));

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float32), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float32), (void*)(3*sizeof(float32)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float32), (void*)(6*sizeof(float32)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  int32 w, h, channels;
  const char* texPath;
  if (HIRES_TEXTURES) {
    texPath = "resources/textures/box.jpg";
  } else {
    texPath = "resources/textures/box128.jpg";
  }
  uint8* data = stbi_load(texPath, &w, &h, &channels, STBI_rgb);

  if (!data) {
    KE_ERROR("Failed to load texture");
  }

  glGenTextures(1, &boxTex);
  glBindTexture(GL_TEXTURE_2D, boxTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  model = scale(mat4(1.0f), vec3(1.5f));

  Renderer::camera = Camera::create(45.0f, vec3(0, 0, 3.0f));
  projection = Renderer::camera->getProjection(Renderer::frameWidth, Renderer::frameHeight);
}

void Renderer::shutdown() {
  Camera::destroy(Renderer::camera);
  Renderer::shader.destroy();
}

void Renderer::startFrame(float deltaTime) {
  Camera::updateAll(deltaTime);
  
  model = rotate(model, 30.0f*deltaTime, vec3(0, 0, 1.0f));
  view = Renderer::camera->getView();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Renderer::shader.use();
  
  Renderer::shader.setMat4("model", model.data());
  Renderer::shader.setMat4("view", view.data());
  Renderer::shader.setMat4("projection", projection.data());

  glBindTexture(GL_TEXTURE_2D, boxTex);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Renderer::endFrame() {
	glFlush();
}

void Renderer::onResize(uint16 width, uint16 height) {
  Renderer::frameWidth = width;
  Renderer::frameHeight = height;
  projection = Renderer::camera->getProjection(Renderer::frameWidth, Renderer::frameHeight);
  glViewport(0, 0, Renderer::frameWidth, Renderer::frameHeight);
}

void Renderer::onKey(uint8 key, bool down) {
  if (key == KEY_ESCAPE && down) {
    Renderer::camera->switchInput();
    InputSystem::setCapture(!InputSystem::getCapture());
  }
}