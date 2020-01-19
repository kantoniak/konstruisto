#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in mat4 in_model;

out vec3 v_position;
out vec3 v_normal;

layout (std140) uniform Camera {
  vec3 position;
	mat4 vp;
} camera;

void main() {
  v_position = vec3(in_model * vec4(in_position, 1.0));
  v_normal = mat3(transpose(inverse(in_model))) * in_normal;
  gl_Position = camera.vp * in_model * vec4(in_position, 1.0);
}