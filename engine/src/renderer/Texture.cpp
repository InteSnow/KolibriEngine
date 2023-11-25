#include "Texture.h"
#include "core/logger.h"
#include GL_HEADER

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture Texture::create(const char* path) {
  Texture tex = {};

  int32 w, h, channels;

  uint8* data = stbi_load(path, &w, &h, &channels, STBI_rgb);
  if (!data) { 
    KE_ERROR("Failed to load texture");
    return tex;
  }

	glGenTextures(1, &tex.handle);
  glBindTexture(GL_TEXTURE_2D, tex.handle);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  return tex;
}

void Texture::destroy(const Texture& tex) {
  glDeleteTextures(1, &tex.handle);
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, this->handle);
}