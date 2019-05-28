#pragma once


#include <cstdlib>

#define CreateError(message, ...) CreateErrorImplementation(__FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)

struct Error
{
    const char* message;
    const char* function;
    const char* file;
    const unsigned line;
};

static const Error* no_error = nullptr;


// @Refactor
// TODO(ted): Probably should rename this.
template <typename Type>
struct Return
{
    Type value;
    const Error* error = no_error;

    Return(Type value) : value(value) {}
    Return(const Error* error) : error(error) {}
};

// Check for errors and terminates the program if an error has occurred.
template <typename Type>
Type Check(const Return<Type>&& x)
{
    if (!x.error)
        return x.value;

    Print(*x.error);

	std::cout << "Press enter to continue..." << std::endl;  // To keep windows window open.
	std::cin.get();

	exit(-1);
}


void Print(const Error& error);
Error* CreateErrorImplementation(const char* function, const char* file, unsigned line, const char* message, ...);