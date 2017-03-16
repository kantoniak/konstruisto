#version 330 core
  
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 position;
layout (location = 2) in int tile;

uniform mat4 transform;
out vec3 vPos;
flat out int vTile;

void main() {
  vPos = vertex + vec3(position.x, 0, position.y);
  vTile = tile;
  gl_Position = transform * vec4(vPos, 1.0);
}