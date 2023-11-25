#include "Model.h"
#include GL_HEADER

void draw(Vertex* vertices, uint32 size) {
  glBegin(GL_TRIANGLES);
  for (uint32 i = 0; i < size; i++) {
    glColor4fv(&vertices[i].color.x);
    glTexCoord2fv(&vertices[i].texCoords.x);
    glVertex3fv(&vertices[i].pos.x);
  }
  glEnd();
}