#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 transform;

out vec3 lighting;

float calcDiffuseAngleStrength(in float ambientStrength, in vec4 v0, in vec4 v1, in vec4 v2) {
    vec3 middle = (v0.xyz + v1.xyz + v2.xyz)/3.f;
    vec3 normal = normalize(cross(v2.xyz-v1.xyz, v0.xyz-v1.xyz));

    vec3 lightDir = normalize(-vec3(-1, 0.7f, -1));
    float diff = max(dot(normal, vec3(-1, 0.7f, -1)), 0.0);
    float diffuseStrength = (1 - ambientStrength) * diff;
    return diffuseStrength;
}

void main() {

    vec3 lightColor = vec3(0.95f, 0.95f, 0.95f);
    vec3 waterColor = vec3(0.5f, 0.5f, 1.f);
    vec3 terrainColor = vec3(0.625f, 0.645f, 0.316f);
    float ambientStrength = 0.7f;

    lighting = (ambientStrength + calcDiffuseAngleStrength(ambientStrength, gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position)) * lightColor;
    for (int i=0; i<3; i++) {
        gl_Position = transform * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}