#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 transform;

void main() {
    vec3 middle = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz)/3.f;
    vec3 normal = normalize(cross(gl_in[2].gl_Position.xyz-gl_in[1].gl_Position.xyz, gl_in[0].gl_Position.xyz-gl_in[1].gl_Position.xyz));

    gl_Position = transform * vec4(middle, 1);
    EmitVertex();

    gl_Position = transform * vec4(middle + normal, 1);
    EmitVertex();

    EndPrimitive();
}