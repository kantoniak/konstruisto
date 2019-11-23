#version 330 core
  
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 size;

void main() {
  gl_Position = vec4(vertex * size + position, 1.0);
}