#include "errors.h"

#include <iostream>
#include <cstring>
#include <cstdarg>


void Print(const Error& error)
{
    fprintf(
            stderr,
            "[User Error] (in '%s'):\n"
            "\tFile:     %s\n"
            "\tLine:     %i\n"
            "\tMessage:  %s\n",
            error.function, error.file, error.line, error.message
    );
}

std::unique_ptr<Error> CreateErrorImplementation(const char* function, const char* file, unsigned line, const char* message, ...)
{
    static const unsigned BUFFER_SIZE = 256;

    char* buffer = new char[BUFFER_SIZE];
    strncpy(buffer, message, BUFFER_SIZE);

    va_list argptr;
    va_start(argptr, message);
    vsnprintf(buffer, BUFFER_SIZE, message, argptr);
    va_end(argptr);

    // TODO(ted): Allocate using pool allocation.
    return std::make_unique<Error>(buffer, function, file, line);
}