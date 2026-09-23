#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec2 a_texCoords;
layout(location = 2) in vec3 a_normals;
layout(location = 3) in vec3 a_colors;

out vec2 v_texCoords;
out vec3 v_normal;
out vec3 v_color;
out float v_distance;

uniform mat4 u_projectionViewMatrix;
uniform mat4 u_modelMatrix = mat4(1.0);

uniform vec3 u_cameraPosition;

uniform vec4 u_uvRegion = vec4(0.0, 0.0, 1.0, 1.0);

void main() {
    vec4 worldPosition = u_modelMatrix * vec4(a_pos, 1.0);

    gl_Position = u_projectionViewMatrix * worldPosition;

    v_texCoords = a_texCoords;

    v_normal = a_normals;
    v_color = a_colors;

    v_distance = length(worldPosition.xyz - u_cameraPosition);
}