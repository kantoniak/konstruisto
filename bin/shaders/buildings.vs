#version 330 core
  
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 position;

uniform mat4 transform;

void main() {
    vec3 v = vertex;
    v.x += gl_InstanceID;
    v.y *= 3 * (1 + sin(float(mod(71 * gl_InstanceID, 11))));
    gl_Position = transform * vec4(v, 1.0);
}