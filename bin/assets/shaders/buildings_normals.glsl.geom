#version 430 core

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 transform;

vec3 getFaceMiddle(in vec4 v0, in vec4 v1, in vec4 v2) {
  return (v0.xyz + v1.xyz + v2.xyz) / 3.f;
}

vec3 getFaceNormal(in vec4 v0, in vec4 v1, in vec4 v2) {
  return normalize(cross(v2.xyz-v1.xyz, v0.xyz-v1.xyz));
}

void main() {
  vec3 middle = getFaceMiddle(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
  vec3 normal = getFaceNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);

  gl_Position = transform * vec4(middle, 1);
  EmitVertex();

  gl_Position = transform * vec4(middle + normal, 1);
  EmitVertex();

  EndPrimitive();
}