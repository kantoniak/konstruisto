#version 330 core

in vec3 vPos;
uniform sampler2D texGround;

out vec4 color;

void main() {
    color = texture(texGround, vPos.xz);
} 