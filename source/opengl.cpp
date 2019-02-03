#include "opengl.h"

#include <iostream>


namespace gl
{
    void ClearErrors() { while (glGetError() != GL_NO_ERROR) {} }

    void PrintErrors(const char* function_name, const char* file_name, int line)
    {
        GLuint error;

        while ((error = glGetError()) != GL_NO_ERROR)
        {
            fprintf(
                stderr,
                "[OpenGL Error] (%s %i):\n\tFunction: %s\n\tFile:     %s\n\tLine:     %i\n",
                ErrorToString(error), error, function_name, file_name, line
            );
        }
    }

    const char* ErrorToString(GLuint error)
    {
        const char* error_message;

        switch (error)
        {
            case GL_INVALID_ENUM:
                error_message = STRINGIFY(GL_INVALID_ENUM);
                break;
            case GL_INVALID_VALUE:
                error_message = STRINGIFY(GL_INVALID_VALUE);
                break;
            case GL_INVALID_OPERATION:
                error_message = STRINGIFY(GL_INVALID_OPERATION);
                break;
            /*
            case GL_STACK_OVERFLOW:
                error_message = STRINGIFY(GL_STACK_OVERFLOW);
                break;
            case GL_STACK_UNDERFLOW:
                error_message = STRINGIFY(GL_STACK_UNDERFLOW);
                break;
            */
            case GL_OUT_OF_MEMORY:
                error_message = STRINGIFY(GL_OUT_OF_MEMORY);
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error_message = STRINGIFY(GL_INVALID_FRAMEBUFFER_OPERATION);
                break;
            default:
                error_message = "UNKNOWN_ERROR";
        }

        return error_message;
    }
}