#version 330 core

in Shared {
    vec3 position;  // World position.
    vec2 texture_coordinate;
    vec3 normal;
} fs_in;


uniform vec3 color;

out vec3 out_color;


void main()
{
    // Hardcoded for now.
    vec3 sunlight_direction = normalize(vec3(1.0f, 1.0f, 1.0f));

    // Ambient light.
    vec3 ambient = color * 0.2f;

    // Specular light.
    float specular_factor = max(dot(fs_in.normal, sunlight_direction), 0);
    vec3  specular = color * specular_factor * 0.8f;

    out_color = ambient + specular;
}
