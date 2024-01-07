#version 120

in vec4 vertexColor;
uniform vec2 dimensions;
uniform float radius;

void main() {
  vec2 coords = (dimensions+1) * (gl_TexCoord[0].xy-0.5);
  
  float alpha = 1.0-max(length(vec2(
  max(abs(coords.x) - (dimensions.x)/2+radius, 0),
  max(abs(coords.y) - (dimensions.y)/2+radius, 0)))-radius, 0.0);

  gl_FragColor = vec4(vertexColor)*vec4(vec3(1.0), alpha);
}