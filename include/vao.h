#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "opengl.h"


struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinate;
    glm::vec3 normal;
};

struct Model
{
    GLuint vao, ebo, count;
};

Model IndexedModel(std::vector<Vertex> vertices, std::vector<GLuint> indices);