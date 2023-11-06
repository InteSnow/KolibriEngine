attribute vec3 vPos;
attribute vec3 vCol;
attribute vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec3 fCol;
varying vec2 fTexCoords;

void main()
{
   gl_Position = projection*view*model*vec4(vPos, 1.0);
   fCol = vCol;
   fTexCoords = vTexCoords;
}  