#pragma once

#include <glad/glad.h>

#define STRINGIFY(x) #x  // TODO(ted): Refactor to another file (used in opengl.cpp).
#define GL_DEBUG 1       // TODO(ted): Should not be defined here.

#if GL_DEBUG
#define GLCALL(statement) gl::ClearErrors(); statement; gl::PrintErrors(#statement, __FILE__, __LINE__)
#else
#define GLCALL(statement) statement
#endif


namespace gl
{
    void ClearErrors();
    void PrintErrors(const char* function_name, const char* file_name, int line);
    const char* ErrorToString(GLuint error);
}
