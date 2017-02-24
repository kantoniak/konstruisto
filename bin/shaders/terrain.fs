#version 330 core

in vec3 vPos;
out vec4 color;

void main() {
    color = vec4(0.625f, 0.645f, 0.316f, 1.0f);

    vec3 pos = vPos;
    if (pos.x - floor(pos.x) < 0.03 || 0.97 < pos.x - floor(pos.x) || pos.z - floor(pos.z) < 0.03 || 0.97 < pos.z - floor(pos.z)) {
        color = color * 0.8f + vec4(0,0,0,1) * 0.2f;
    }
} 