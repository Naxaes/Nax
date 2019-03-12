#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture_coordinate;
layout (location = 2) in vec3 normal;

struct SunLight
{
    vec4 direction;
    vec4 color;
};

uniform mat4 model;
layout (std140) uniform Data
{
    mat4 view;
    mat4 projection;

    vec4 color;
    SunLight sunlight;

    float ambient_factor;
    float diffuse_factor;
    float specular_factor;
    float shininess;
};

out Shared {
    vec3 position;  // World space.
    vec2 texture_coordinate;
    vec3 normal;    // World space.
} vs_out;



void main()
{
    // Send to fragment.
    vs_out.position = vec3(model * vec4(position, 1.0f));
    vs_out.texture_coordinate = texture_coordinate;
    vs_out.normal = vec3(normalize(model * vec4(normal, 0.0f)));

    // Vertex position on screen.
    gl_Position = projection * view * model * vec4(position, 1.0f);
}
