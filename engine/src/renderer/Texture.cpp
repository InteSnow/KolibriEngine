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

Texture Texture::create(std::string path) {
  Texture tex = {};

  int32 w, h, channels;
  glActiveTexture(GL_TEXTURE0);
  uint8* data = texLoad(path.c_str(), &w, &h, &channels, STBI_rgb);
  if (!data) { 
    KE_ERROR("Failed to load texture");
    return tex;
  }

  tex = Texture::create(data, w, h, 3);

  texFree(data);

  return tex;
}

Texture Texture::create(uint8* bitmap, int w, int h, int channels) {
  Texture tex;
  GLenum format = channels == 3 ? GL_RGB : GL_ALPHA;
  GLint components;
  #ifdef KE_PLATFORM_WIN32
  components = format;
  //components = channels;
  #else
  components = channels;
  #endif
  glGenTextures(1, &tex.handle);

  tex.bind(0);
  
  glTexImage2D(GL_TEXTURE_2D, 0, components, w, h, 0, format, GL_UNSIGNED_BYTE, bitmap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  tex.unbind();

  return tex;
}

void Texture::destroy(const Texture& tex) {
  glDeleteTextures(1, &tex.handle);
}

void Texture::bind(int32 slot) {
  this->slot = slot;
  glActiveTexture(GL_TEXTURE0+this->slot);
  glBindTexture(GL_TEXTURE_2D, this->handle);
}

void Texture::unbind() {
  glActiveTexture(GL_TEXTURE0+this->slot);
  glBindTexture(GL_TEXTURE_2D, 0);
}
