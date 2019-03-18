#include "vao.h"

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include "opengl.h"


Mesh Cube()
{
    std::vector<Vertex> vertices = {
            //    Positions          Texture coordinates           Normals
            {{-0.5f, -0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Left bottom
            {{ 0.5f,  0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Right top
            {{-0.5f,  0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Left  top
            {{ 0.5f, -0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Right bottom
    };
    std::vector<GLuint> indices = {
            0, 1, 2,
            0, 3, 1
    };

    return IndexedModel(vertices, indices);
}


Mesh IndexedModel(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
    GLuint vao, vbo, ebo;

    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glGenBuffers(1, &vbo));
    GLCALL(glGenBuffers(1, &ebo));

    GLCALL(glBindVertexArray(vao));

    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW));

    // Set the vertex attribute pointers
    // Positions
    GLCALL(glEnableVertexAttribArray(0));
    GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

    // Texture coordinates
    GLCALL(glEnableVertexAttribArray(1));
    GLCALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate)));

    // Normals
    GLCALL(glEnableVertexAttribArray(2));
    GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));

    GLCALL(glBindVertexArray(0));

    return {vao, ebo, static_cast<GLuint>(indices.size()), vertices};
}
