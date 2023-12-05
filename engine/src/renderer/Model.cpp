#include "Model.h"
#include GL_HEADER

Model Model::create(Vertex* vertices, uint32 size, const char* diffuse, ShadingType shading) {
  Model model;

  model.vertices = vertices;
  model.size = size;
  model.shading = shading;

  if (diffuse) {
    model.hasDT = 1;
    model.diffTex = Texture::create(diffuse);
  } else {
    model.hasDT = 0;
  }
  
  model.color = vec3(1.0f);

  return model;
}
void Model::draw(void) {
  if (!this->vertices) return;

  if (this->shading == SHADE_LIT) {
    glEnable(GL_LIGHTING);
  } else {
    glDisable(GL_LIGHTING);
  }

  if (this->hasDT) {
    glEnable(GL_TEXTURE_2D);
    this->diffTex.bind(0);
  } else {
    glDisable(GL_TEXTURE_2D);
  }

  vec3 color;
  glBegin(GL_TRIANGLES);
  for (uint32 i = 0; i < this->size; i++) {
    color = this->color * this->vertices[i].color;
    glNormal3fv(&this->vertices[i].normal.x);
    glColor3fv(&color.x);
    glTexCoord2fv(&this->vertices[i].texCoords.x);
    glVertex3fv(&this->vertices[i].pos.x);
  }
  glEnd();

  if (this->hasDT) {
    this->diffTex.unbind();
  }
}

void Model::destroy(Model& model) {
  if (model.hasDT) {
    Texture::destroy(model.diffTex);
  }
}

void Model::setColor(vec3 color) {
  this->color = clamp(color, vec3(0.0f), vec3(1.0f));
}
vec3 Model::getColor() {
  return this->color;
}

void Model::onDraw() {
  this->draw();
}
#include "core/logger.h"
void Model::onUnregister() {
  Model::destroy(*this);
}
