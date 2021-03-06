#include "debug.h"

#include <iostream>
#include <cstring>
#include <cstdarg>


void AssertImplementation(
        bool status,
        const char* statement,
        const char* file,
        unsigned line,
        const char* message,
        ...
)
{
    if (status)
        return;

    static const unsigned BUFFER_SIZE = 256;

    char* buffer = new char[BUFFER_SIZE];
    strncpy(buffer, message, BUFFER_SIZE);

    va_list argptr;
    va_start(argptr, message);
    vsnprintf(buffer, BUFFER_SIZE, message, argptr);
    va_end(argptr);

    fprintf(
            stderr,
            "[Assertion Error] (%s):\n"
            "\tFile:     %s\n"
            "\tLine:     %i\n"
            "\tMessage:  %s\n",
            statement, file, line, buffer
    );

	std::cout << "Press enter to continue..." << std::endl;  // To keep windows window open.
	std::cin.get();

    exit(-1);
}

