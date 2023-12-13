#include "Mesh.h"
#include GL_HEADER

Mesh Mesh::create(std::vector<Vertex>&& vertices, std::string diffuse) {
  Mesh mesh;

  mesh.vertices = vertices;

  mesh.hasDT = !diffuse.empty();
  if (mesh.hasDT) {
    mesh.diffTex = Texture::create(diffuse);
  }
  
  return mesh;
}

void Mesh::draw(void) {
  if (!this->vertices.size()) return;

  if (this->hasDT) {
    glEnable(GL_TEXTURE_2D);
    this->diffTex.bind(0);
  } else {
    glDisable(GL_TEXTURE_2D);
  }

  vec3 color;
  glBegin(GL_TRIANGLES);
  for (uint32 i = 0; i < this->vertices.size(); i++) {
    color = this->vertices[i].color;
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

void Mesh::destroy(Mesh& mesh) {
  if (mesh.hasDT) {
    Texture::destroy(mesh.diffTex);
  }
}