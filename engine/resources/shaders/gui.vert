#version 120

varying vec4 vertexColor;

void main() {
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  vertexColor = gl_Color;
  gl_TexCoord[0] = gl_MultiTexCoord0;
}