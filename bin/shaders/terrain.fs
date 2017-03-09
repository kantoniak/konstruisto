#version 330 core

uniform sampler2D texGround;
uniform ivec4 selection;
uniform vec4 selectionColor;

in vec3 vPos;

out vec4 color;

void main() {
    vec2 selectionStart = selection.xy;
    vec2 selectionStop = selection.zw;
    
    color = texture(texGround, vPos.xz);

    if (selectionStart.x <= vPos.x && vPos.x <= selectionStop.x && selectionStart.y <= vPos.z && vPos.z <= selectionStop.y) {
        color = mix(color, vec4(selectionColor.xyz, 1), selectionColor.w);
    }
} 