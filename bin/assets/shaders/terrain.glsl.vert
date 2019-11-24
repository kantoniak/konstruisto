#version 430 core
  
layout (location = 0) in vec3 vertex;
layout (location = 1) in float tile;

uniform mat4 transform;
out vec3 vPos;
flat out int vTile;

void main() {
  vPos = vertex;
  vTile = int(tile) - 1;
  gl_Position = transform * vec4(vPos, 1.0);
}