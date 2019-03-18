#pragma once

#include <vector>
#include <string>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "opengl.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinate;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    GLuint id;
    std::string type;
};

struct Mesh
{
    GLuint vao, ebo, count;

    // TODO(ted): Should we have this? Keeping it now for debugging purposes.
    std::vector<Vertex> vertices;
};

struct TexturedMesh
{
    Mesh mesh;
    std::vector<Texture> textures;
};

struct TexturedModel
{
    std::vector<TexturedMesh> meshes;
};



Mesh IndexedModel(std::vector<Vertex> vertices, std::vector<GLuint> indices);
Mesh Cube();