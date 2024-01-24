#include "Mesh.h"
#include GL_HEADER

Mesh Mesh::create(std::vector<Vertex>&& vertices, ShadingType shading, std::string diffuse) {
  Mesh mesh;

  mesh.vertices = vertices;
  mesh.shading = shading;

  mesh.hasDT = !diffuse.empty();
  if (mesh.hasDT) {
    mesh.diffTex = Texture::create(diffuse);
  }
  
  return mesh;
}

void Mesh::onDraw(void) {
  if (!this->vertices.size()) return;

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

void Mesh::onUnregister() {
  Mesh::destroy(*this);
}

void Mesh::destroy(Mesh& mesh) {
  if (mesh.hasDT) {
    Texture::destroy(mesh.diffTex);
  }
}

BoundingBox Mesh::getBoundingBox() const {
  BoundingBox b = {};
  for (uint32 i = 0; i < this->vertices.size(); i++) {
    const Vertex& v = this->vertices[i];
    if (!i) {
      b.min = v.pos;
      b.max = v.pos;
      continue;
    }    
    b.min.x = b.min.x > v.pos.x ? v.pos.x : b.min.x;
    b.min.y = b.min.y > v.pos.y ? v.pos.y : b.min.y;
    b.min.z = b.min.z > v.pos.z ? v.pos.z : b.min.z;
    b.max.x = b.max.x < v.pos.x ? v.pos.x : b.max.x;
    b.max.y = b.max.y < v.pos.y ? v.pos.y : b.max.y;
    b.max.z = b.max.z < v.pos.z ? v.pos.z : b.max.z;
  }
  return b;
}