#version 330 core
  
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 position;

void main() {
    vec3 v;
    v.x = vertex.x + position.x;
    v.z = vertex.z + position.z;
    v.y = vertex.y * 0.5f * position.y;
    gl_Position = vec4(v, 1.0);
}