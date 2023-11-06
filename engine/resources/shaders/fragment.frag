varying vec3 fCol;
varying vec2 fTexCoords;

uniform sampler2D texture;

void main() {
  gl_FragColor = vec4(mix(fCol, vec3(1.0), .7), 1.0) * texture2D(texture, fTexCoords);
} 