#version 330 core

uniform sampler2D groundTexture;
uniform sampler2D roadTexture;

uniform bool renderGrid;
uniform ivec4 selection;
uniform vec4 selectionColor;

in vec3 vPos;
flat in int vTile;

out vec4 color;

void main() {
  vec2 selectionStart = selection.xy;
  vec2 selectionStop = selection.zw;

  color = vec4(0.624f, 0.643f, 0.318f, 1);

  if (vTile == 0) {
    color = vec4(0.2f, 0.2f, 0.2f, 1);
  }
  vec4 roadColor = texture(roadTexture, vPos.xz / 3.f);
  color = vec4(mix(color.xyz, roadColor.xyz, int(vTile == 0) * roadColor.w), 1);

  if (renderGrid) {
    vec4 gridColor = texture(groundTexture, vPos.xz / 16.f + vec2(0.5, 0.5) / 512.f);
    color = vec4(mix(color.xyz, gridColor.xyz, gridColor.w), 1);
  }

  if (selectionStart.x <= vPos.x && vPos.x <= selectionStop.x && selectionStart.y <= vPos.z && vPos.z <= selectionStop.y) {
    color = mix(color, vec4(selectionColor.xyz, 1), selectionColor.w);
  }
} 