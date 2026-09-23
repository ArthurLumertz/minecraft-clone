#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_texCoords;
in vec3 v_normal;
in vec3 v_color;
in float v_distance;

uniform sampler2D u_sampler;

uniform bool u_useSolidColor = false;
uniform vec4 u_solidColor = vec4(1.0);

uniform bool u_useFog = true;
uniform bool u_useLighting = true;
uniform vec3 u_fogColor;

uniform vec3 u_sunDirection;

void main() {
    if (u_useSolidColor) {
        FragColor = u_solidColor;
        return;
    }

    vec4 color = texture(u_sampler, v_texCoords);

    // Do this regardless of fog
    if (color.a < 0.5) {
        discard;
    }

    // Directional sunlight
    if (u_useLighting) {
        vec3 normal = normalize(v_normal);
        vec3 sunDir = normalize(u_sunDirection);

        float diffuse = max(
            dot(normal, sunDir),
            0.0
        );

        // Prevent unlit faces from becoming completely black
        float ambient = 0.45;

        float brightness =
            ambient +
            diffuse * (1.0 - ambient);

        color.rgb *= brightness;
    }

    // Fog should generally happen AFTER lighting
    if (u_useFog) {
        float fogStart = 96.0;
        float fogEnd = 128.0;

        float fog = clamp(
            (v_distance - fogStart) /
            (fogEnd - fogStart),
            0.0,
            1.0
        );

        color.rgb = mix(
            color.rgb,
            u_fogColor,
            fog
        );
    }

    FragColor = color;
}