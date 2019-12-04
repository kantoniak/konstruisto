#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

out vec3 v_position;
out vec3 v_normal;

layout (std140) uniform Camera {
  vec3 position;
	mat4 vp;
} camera;

uniform mat4 model;

void main() {
  v_position = vec3(model * vec4(in_position, 1.0));
  v_normal = mat3(transpose(inverse(model))) * in_normal;
  gl_Position = camera.vp * model * vec4(in_position, 1.0);
}