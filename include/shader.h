#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


enum class ShaderType
{
    VERTEX   = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER
};


// ---- STRUCTS ----

// Debugging info for shaders.
struct ShaderInfo
{
    const ShaderType  type;
    const std::string name;         // Should we be able to change the name?
    const std::string source;
};

// Since shaders can be shared by multiple shader programs, we should only free the shader info when no programs
// are referencing the shader.
struct Shader
{
    const GLuint id;
    const std::shared_ptr<ShaderInfo> info;
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

// ShadersProgram owns its own info object, hence info should be deleted when the program gets deleted.
struct ShaderProgram
{
    const GLuint id;
    const ShaderProgramInfo* info;

    ~ShaderProgram() { delete info; }
};


// Strong type wrapper.
struct UniformLocation { GLuint id; };


// ---- FUNCTIONS ----


// Creates shader of 'type' and checks if it compiles correctly.
Shader CreateShader(std::string source, ShaderType type, std::string name = "");
// Creates shader program and make sure it links properly.
ShaderProgram CreateShaderProgram(const std::vector<Shader>& shaders, const std::vector<std::string>& attributes, std::string name = "");

// Deletes shader
void DeleteShader(GLuint shader);
// Deletes shader program
void DeleteShaderProgram(GLuint program);

// Enables a ShaderProgram in order to set or cache uniforms.
void Enable(const ShaderProgram& program);

// Returns an id to the uniform. NOTE: ShaderProgram must be enabled using Enable(ShaderProgram);
UniformLocation CacheUniform(const ShaderProgram& program, std::string name);

// Overloads for setting a uniform. NOTE: ShaderProgram must be enabled using Enable(ShaderProgram);
void SetUniform(UniformLocation uniform, float value);
void SetUniform(UniformLocation uniform, glm::vec2 value);
void SetUniform(UniformLocation uniform, glm::vec3 value);
void SetUniform(UniformLocation uniform, glm::vec4 value);
void SetUniform(UniformLocation uniform, glm::mat4 value);



// The ones below is used internally.


// int  ConfirmShaderStatus(GLuint shader, GLuint status);
// void PrintShaderErrors(GLuint shader, GLuint status, std::string name);
// int  ConfirmProgramStatus(GLuint program, GLuint status);
// void PrintProgramErrors(GLuint program, GLuint status, std::string status_name, std::string name);

// // Fetches all attributes from a program.
// std::vector<GLSLAttribute> GetActiveAttributes(GLuint program);
// // Fetches all uniforms from a program.
// std::vector<GLSLUniform>   GetActiveUniforms(GLuint program);
