#version 430 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140) uniform Camera {
	mat4 vp;
} camera;

out vec3 lighting;

vec3 getFaceNormal(in vec4 v0, in vec4 v1, in vec4 v2) {
  return normalize(cross(v2.xyz-v1.xyz, v0.xyz-v1.xyz));
}

float calcLambertian(in vec3 lightDir, in vec4 v0, in vec4 v1, in vec4 v2) {
  vec3 normal = getFaceNormal(v0, v1, v2);
  return max(dot(normal, lightDir), 0.0);
}

void main() {
    vec3 lightDir = vec3(-1, 0.7f, 0.8f);
    vec3 lightColor = vec3(0.95f, 0.95f, 0.95f);
    float ambientStrength = 0.7f;
    float diffuseStrength = 0.3f;

    lighting = (ambientStrength + diffuseStrength * calcLambertian(lightDir, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position)) * lightColor;
    for (int i=0; i<3; i++) {
        gl_Position = camera.vp * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}