#version 330 core

in Shared {
    vec3 position;  // World position.
    vec2 texture_coordinate;
    vec3 normal;
} fs_in;


uniform vec3 color;
uniform vec3 sunlight_direction;

out vec3 out_color;


void main()
{
    // Pedantic safety.
    vec3 normalized_sunlight_direction = normalize(sunlight_direction);

    // Ambient light.
    vec3 ambient = color * 0.2f;

    // Specular light.
    float specular_factor = max(dot(fs_in.normal, normalized_sunlight_direction), 0);
    vec3  specular = color * specular_factor * 0.8f;

    out_color = ambient + specular;
}
