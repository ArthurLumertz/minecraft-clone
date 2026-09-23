#version 330 core

layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_texCoords;

uniform mat4 u_projectionViewMatrix;
uniform mat4 u_modelMatrix = mat4(1.0);

uniform vec4 u_uvRegion = vec4(0.0, 0.0, 1.0, 1.0);

out vec2 v_texCoords;

void main() {
	gl_Position = u_projectionViewMatrix * u_modelMatrix * vec4(a_pos, 0.0, 1.0);
	v_texCoords = mix(
        u_uvRegion.xy,
        u_uvRegion.zw,
        a_texCoords
    );
}