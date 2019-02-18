#version 330 core

in Shared {
    vec3 position;  // World space.
    vec2 texture_coordinate;
    vec3 normal;    // World space.
} fs_in;


uniform mat4 view;

uniform vec3 color;
uniform vec3 sunlight_direction;

uniform float ambient_factor;
uniform float diffuse_factor;
uniform float specular_factor;
uniform float shininess;

out vec3 out_color;


void main()
{

    // Variable setup.
    vec3 sunlight_position = -sunlight_direction * 1000.0f;
    vec3 camera_position   = view[3].xyz;

    vec3 fragment_to_light_direction  = normalize(sunlight_position - fs_in.position);
    vec3 fragment_to_camera_direction = normalize(camera_position   - fs_in.position);

    vec3 light_color = vec3(1.0f);

    // Ambient light.
    vec3 ambient = light_color * ambient_factor;

    // Diffues light.
    float sunlight_normal_angle = max(dot(fs_in.normal, fragment_to_light_direction), 0);
    vec3  diffuse = light_color * sunlight_normal_angle * diffuse_factor;

    // Specular light.
    vec3  halfway_direction = normalize(fragment_to_light_direction + fragment_to_camera_direction);
    vec3  reflected_light_direction = reflect(fragment_to_light_direction, fs_in.normal);
    float specular_angle = max(dot(reflected_light_direction, fragment_to_camera_direction), 0.0f);
    vec3  specular = light_color * pow(specular_angle, shininess) * specular_factor;

    // Output.
    out_color = (ambient + diffuse + specular) * color;
}
