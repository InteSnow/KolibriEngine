#include "Model.h"
#include GL_HEADER

void draw(Vertex* vertices, uint32 size) {
  glBegin(GL_TRIANGLES);
  for (uint32 i = 0; i < size; i++) {
    glNormal3fv(&vertices[i].normal.x);
    // TODO: implement combining diffuse map with color 
    //glColor4fv(&vertices[i].color.x);
    glColor3f(1, 1, 1);
    glTexCoord2fv(&vertices[i].texCoords.x);
    glVertex3fv(&vertices[i].pos.x);
  }
  glEnd();
}