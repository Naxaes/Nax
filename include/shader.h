#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "opengl.h"
#include "debug.h"


enum class ShaderType
{
    VERTEX   = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER
};


// ---- STRUCTS ----

struct Shader
{
    const GLuint id;
    const GLuint info_index;
};

struct ShaderProgram
{
    const GLuint id;
    const GLuint info_index;
};




// Debugging info for shaders.
struct ShaderInfo
{
    const ShaderType  type;
    const std::string name;         // Should we be able to change the name?
    const std::string source;
};

// Debugging info for shader attributes.
struct GLSLAttribute
{
    const std::string name;
    const GLenum type;
};

// Debugging info for shader uniforms.
struct GLSLUniform
{
    const std::string name;
    const GLenum type;
};

// Debugging info for programs.
struct ShaderProgramInfo
{
    const std::string name;
    const std::vector<Shader>         shaders;
    const std::vector<GLSLAttribute>  attributes;
    const std::vector<GLSLUniform>    uniforms;
};


// Strong type wrapper.
struct UniformLocation { GLuint id; };




// ---- SHADER FUNCTIONS ----
ShaderInfo& GetShaderInfo(Shader shader);
ShaderProgramInfo& GetShaderProgramInfo(ShaderProgram program);


// Creates shader of 'type' and checks if it compiles correctly.
Shader CreateShader(std::string source, ShaderType type, std::string name = "");
// Creates shader program and make sure it links properly.
ShaderProgram CreateShaderProgram(const std::vector<Shader>& shaders, const std::vector<std::string>& attributes, std::string name = "");

// Deletes shader
void DeleteShader(GLuint shader);
// Deletes shader program
void DeleteShaderProgram(GLuint program);

// Sets the program as active.
void Enable(ShaderProgram program);


// ---- UNIFORMS ----

// Returns an id to the uniform. NOTE: ShaderProgram must be enabled using Enable(ShaderProgram);
UniformLocation CacheUniform(ShaderProgram program, std::string name);

// Overloads for setting a uniform. NOTE: ShaderProgram must be enabled using Enable(ShaderProgram);
void SetUniform(UniformLocation uniform, float value);
void SetUniform(UniformLocation uniform, glm::vec2 value);
void SetUniform(UniformLocation uniform, glm::vec3 value);
void SetUniform(UniformLocation uniform, glm::vec4 value);
void SetUniform(UniformLocation uniform, glm::mat4 value);




// ---- UNIFORM BUFFERS ----

// TODO(ted): This template hurts me. Now I have to implement the functions here in the header which forces me to
//    include "debug.h" and "opengl.h". But I feel it's necessary since we want to be able to access attributes in the
//    data member. It also allow us to remove a macro that passed the data and size to the CreateBuffer function.
template<typename Type>
struct UniformBuffer
{
    GLuint id;
    Type data;

    // TODO(ted): Type is suppose to fulfill GLSL's std140 layout requirements.
    //    [https://www.khronos.org/registry/OpenGL/specs/gl/glspec45.core.pdf#page=159]
    //    Warning: You are advised to manually pad your structures/arrays out and avoid using vec3 at all.
};


template<typename Type>
UniformBuffer<Type> CreateUniformBuffer(const Type& data)
{
    constexpr unsigned size = sizeof(Type);

    Assert(size % 4 == 0, "All members in data must be a multiple of 4! Size is %i. Pad the members!", size);

    GLuint buffer;
    GLCALL(glGenBuffers(1, &buffer));
    GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, buffer));
    GLCALL(glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));

    return {buffer, data};
}

template<typename Type>
void SetUniformBuffer(UniformBuffer<Type> uniform_buffer, unsigned offset = 0)
{
    constexpr unsigned size = sizeof(Type);

    // LOAD (global)
    GLCALL(glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer.id));
    GLCALL(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, &uniform_buffer.data));
}


GLuint AllocateUniformBuffer(unsigned size);
void   SetUniformBuffer(GLuint uniform_block, unsigned size, void* data, unsigned offset = 0);
void   AddUniformBuffer(ShaderProgram program, const char* uniform_block_name, GLuint uniform_block_name_id);