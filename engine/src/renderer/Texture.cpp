#include "Texture.h"
#include "core/logger.h"
#include GL_HEADER

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static uint8* texLoad(const char* path, int32* w, int32* h, int32* channels, int stbFormat) {
  uint8* data = stbi_load(path, w, h, channels, stbFormat);
  if (!data) { 
    return NULL;
  }

  return data;
}

static void texFree(uint8* data) {
  stbi_image_free(data);
}

Texture Texture::create(const char* path) {
  Texture tex = {};

  int32 w, h, channels;
  glActiveTexture(GL_TEXTURE0);
  uint8* data = texLoad(path, &w, &h, &channels, STBI_rgb);
  if (!data) { 
    KE_ERROR("Failed to load texture");
    return tex;
  }

	glGenTextures(1, &tex.handle);

  tex.bind(0);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  tex.unbind();

  texFree(data);

  return tex;
}

void Texture::destroy(const Texture& tex) {
  glDeleteTextures(1, &tex.handle);
}

void Texture::bind(int slot) {
  this->slot = slot;
  glActiveTexture(GL_TEXTURE0+this->slot);
  glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::unbind() {
  glActiveTexture(GL_TEXTURE0+this->slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
