#version 430 core

layout (std140) uniform Camera {
    vec3 position;
	mat4 vp;
} camera;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform vec3 light_pos;

// Input
in vec3 v_position;
in vec3 v_normal;

// Output
layout(location = 0) out vec4 color;

void main() {
    vec3 lightColor = vec3(1.f, 1.f, 1.f);
    vec3 lighPosCor = vec3(light_pos.x, light_pos.y, light_pos.z);

    // ambient
    vec3 ambient = lightColor * material.ambient;

    // diffuse 
    vec3 norm = normalize(v_normal);
    //vec3 lightDir = normalize(lighPosCor - v_position);
    vec3 lightDir = vec3(-1, 0.7f, 0.8f);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);

    // specular
    vec3 viewDir = normalize(camera.position - v_position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}