#include "shader.h"

#include "shader.h"

#include <glad/glad.h>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

#include "opengl.h"


// Forward declaration of internal functions.
int  ConfirmShaderStatus(GLuint shader, GLuint status);
void PrintShaderErrors(GLuint shader, GLuint status, std::string name);
int  ConfirmProgramStatus(GLuint program, GLuint status);
void PrintProgramErrors(GLuint program, GLuint status, std::string status_name, std::string name);
std::vector<GLSLAttribute> GetActiveAttributes(GLuint program);
std::vector<GLSLUniform> GetActiveUniforms(GLuint program);


// Creates shader of 'type' and checks if it compiles correctly.
Shader CreateShader(std::string source, ShaderType type, std::string name)
{
    const char* c_source = source.c_str();

    GLCALL(GLuint id = glCreateShader(static_cast<GLenum>(type)));
    GLCALL(glShaderSource(id, 1, &c_source, nullptr));
    GLCALL(glCompileShader(id));

    if (name.empty())
        name = "Shader " + std::to_string(id);

    if (!ConfirmShaderStatus(id, GL_COMPILE_STATUS))
        PrintShaderErrors(id, GL_COMPILE_STATUS, name);

    // TODO(ted): ShaderProgramInfo has no constructor. Must create temporary object to 'make_shared'.
    return {.id = id, .info = std::make_shared<ShaderInfo>(ShaderInfo{type, name, source})};
}



// Creates shader program and make sure it links properly.
ShaderProgram CreateShaderProgram(
    const std::vector<Shader>& shaders,
    const std::vector<std::string>& attributes,
    std::string name
)
{
    GLuint id;
    GLCALL(id = glCreateProgram());

    // Attach and bind attributes.
    for (const Shader& shader : shaders) { GLCALL(glAttachShader(id, shader.id)); }
    for (unsigned i = 0; i < attributes.size(); i++) { GLCALL(glBindAttribLocation(id, i, attributes[i].c_str())); }

    GLCALL(glLinkProgram(id));

    // Debug.
    if (!ConfirmProgramStatus(id, GL_LINK_STATUS))
        PrintProgramErrors(id, GL_LINK_STATUS, "GL_LINK_STATUS", name);

    // // TODO(ted): Save to delete shaders on graphic card, but in the future we might want to reuse shaders.
    // for (const Shader& shader : shaders) { GLCALL(glDeleteShader(shader.id)); }


    // Cache attributes and uniforms.
    std::vector<GLSLUniform>   active_uniforms   = GetActiveUniforms(id);
    std::vector<GLSLAttribute> active_attributes = GetActiveAttributes(id);

    return { .id = id, .info = new ShaderProgramInfo{name, shaders, active_attributes, active_uniforms} };
}



std::vector<GLSLAttribute> GetActiveAttributes(GLuint program)
{
    GLint count;
    GLCALL(glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count));

    constexpr unsigned max_name_size = 128;  // Safe to assume no name is greater than 128.

    GLchar name[max_name_size];
    GLint  length;
    GLint  size;
    GLenum type;

    std::vector<GLSLAttribute> attributes;
    attributes.reserve(count);

    for (GLuint i = 0; i < count; ++i)
    {
        GLCALL(glGetActiveAttrib(program, i, max_name_size, &length, &size, &type, name));
        attributes.emplace_back(GLSLAttribute{name, type});  // TODO(ted): GLSLAttribute has no constructor.
    }

    return attributes;
}


std::vector<GLSLUniform> GetActiveUniforms(GLuint program)
{
    GLint count;
    GLCALL(glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count));

    constexpr unsigned max_name_size = 128;  // Safe to assume no name is greater than 128.

    GLchar name[max_name_size];
    GLint  length;
    GLint  size;
    GLenum type;

    std::vector<GLSLUniform> uniforms;
    uniforms.reserve(count);

    for (GLuint i = 0; i < count; ++i)
    {
        GLCALL(glGetActiveUniform(program, i, max_name_size, &length, &size, &type, name));
        uniforms.emplace_back(GLSLUniform{name, type});  // TODO(ted): GLSLUniform has no constructor.
    }

    return uniforms;
}


int ConfirmShaderStatus(GLuint shader, GLuint status)
{
    GLint success;
    GLCALL(glGetShaderiv(shader, status, &success));
    return success;
}

void PrintShaderErrors(GLuint shader, GLuint status, std::string name)
{
    int log_size;
    GLCALL(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size));

    GLchar* log = new char[log_size];
    GLCALL(glGetShaderInfoLog(shader, log_size, nullptr, log));

    fprintf(
            stderr,
            "[OpenGL Shader Error]:\n"
            "\tShader:  %s\n"
            "\tStatus:  %s\n"
            "\tResult:  %i\n"
            "\tMessage: %s\n",

            name.c_str(), "GL_COMPILE_STATUS", status, log
    );
}


int ConfirmProgramStatus(GLuint program, GLuint status)
{
    GLint success;
    GLCALL(glGetProgramiv(program, status, &success));
    return success;
}

void PrintProgramErrors(GLuint program, GLuint status, std::string status_name, std::string name)
{
    int log_size;
    GLCALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size));

    GLchar* log = new char[log_size];
    GLCALL(glGetProgramInfoLog(program, log_size, nullptr, log));

    fprintf(
            stderr,
            "[OpenGL Shader Program Error]:\n"
            "\tProgram: %s\n"
            "\tStatus:  %s\n"
            "\tResult:  %i\n"
            "\tMessage: %s\n",

            name.c_str(), status_name.c_str(), status, log
    );
}


void Enable(const ShaderProgram& program)
{
    GLCALL(glUseProgram(program.id));
}


UniformLocation CacheUniform(const ShaderProgram& program, std::string name)
{
    GLCALL(GLint location = glGetUniformLocation(program.id, name.c_str()));
    if (location < 0)
        throw std::runtime_error("Location " + name + " for program " + program.info->name + " was not found!");
    return { static_cast<GLuint>(location) };
};

void SetUniform(UniformLocation uniform, float value)
{
    GLCALL(glUniform1f(uniform.id, value));
}
void SetUniform(UniformLocation uniform, glm::vec2 value)
{
    GLCALL(glUniform2f(uniform.id, value.x, value.y));
}
void SetUniform(UniformLocation uniform, glm::vec3 value)
{
    GLCALL(glUniform3f(uniform.id, value.x, value.y, value.z));
}
void SetUniform(UniformLocation uniform, glm::vec4 value)
{
    GLCALL(glUniform4f(uniform.id, value.x, value.y, value.z, value.w));
}
void SetUniform(UniformLocation uniform, glm::mat4 value)
{
    GLCALL(glUniformMatrix4fv(uniform.id, 1, GL_FALSE, &value[0][0]));
}