#version 330 core

in Shared {
    vec3 position;  // World space.
    vec2 texture_coordinate;
    vec3 normal;    // World space.
} fs_in;


struct SunLight
{
    vec4 direction;
    vec4 color;
};


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

uniform uint diffuse_count;
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;

uniform uint specular_count;
uniform sampler2D texture_specular0;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;

out vec4 out_color;


void main()
{

    // Variable setup.
    vec3 sunlight_position = -vec3(sunlight.direction) * 1000.0f;
    vec3 camera_position   = view[3].xyz;

    vec3 fragment_to_light_direction  = normalize(sunlight_position - fs_in.position);
    vec3 fragment_to_camera_direction = normalize(camera_position   - fs_in.position);

	vec4 diffuse_color = vec4(0);
	vec4 specular_color = vec4(0);

	if ((diffuse_count + specular_count) == 0u)
	{
		diffuse_color  = sunlight.color;
		specular_color = sunlight.color;
	}
	else
	{
		// Diffuse color
		diffuse_color += texture(texture_diffuse0, fs_in.texture_coordinate) * float(diffuse_count > 0u);
		diffuse_color += texture(texture_diffuse1, fs_in.texture_coordinate) * float(diffuse_count > 1u);
		diffuse_color += texture(texture_diffuse2, fs_in.texture_coordinate) * float(diffuse_count > 2u);
		diffuse_color += texture(texture_diffuse3, fs_in.texture_coordinate) * float(diffuse_count > 3u);
		if (length(diffuse_color) > 0)
			diffuse_color = normalize(diffuse_color);

		// Specular color
		specular_color += texture(texture_specular0, fs_in.texture_coordinate) * float(specular_count > 0u);
		specular_color += texture(texture_specular1, fs_in.texture_coordinate) * float(specular_count > 1u);
		specular_color += texture(texture_specular2, fs_in.texture_coordinate) * float(specular_count > 2u);
		specular_color += texture(texture_specular3, fs_in.texture_coordinate) * float(specular_count > 3u);
		if (length(diffuse_color) > 0)
			diffuse_color = normalize(diffuse_color);
	}



    // Ambient light.
    vec4 ambient = diffuse_color * sunlight.color * ambient_factor;

    // Diffuse light.
    float sunlight_normal_angle = max(dot(fs_in.normal, fragment_to_light_direction), 0);
    vec4  diffuse = diffuse_color * sunlight.color * sunlight_normal_angle * diffuse_factor;

    // Specular light.
    vec3  halfway_direction = normalize(fragment_to_light_direction + fragment_to_camera_direction);
    vec3  reflected_light_direction = reflect(fragment_to_light_direction, fs_in.normal);
    float specular_angle = max(dot(reflected_light_direction, fragment_to_camera_direction), 0.0f);
    vec4  specular = specular_color * sunlight.color * pow(specular_angle, shininess) * specular_factor;

    // Output.
    out_color = (ambient + diffuse + specular);
}
