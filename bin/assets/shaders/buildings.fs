#version 330 core

in vec3 lighting;
out vec4 color;

void main() {
    // TODO(kantoniak) : Implement SSAO
    color = vec4(lighting, 1.0f);
} 