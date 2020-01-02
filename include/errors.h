#pragma once


#include <cstdlib>
#include <iostream>
#include <memory>

#define CreateError(message, ...) CreateErrorImplementation(__FUNCTION__, __FILE__, __LINE__, message, __VA_ARGS__)

class Error
{
public:
    const char* message;
    const char* function;
    const char* file;
    const unsigned line;

	Error(const char* message, const char* function, const char* file, const unsigned line) :
		message(message), function(function), file(file), line(line) {}
};

// @Refactor
// TODO(ted): Probably should rename this.
template <typename Type>
struct Return
{
    Type value;
    const std::unique_ptr<Error> error;

    Return(Type value) : value(value), error(nullptr) {}
    Return(std::unique_ptr<Error>&& error) : value(), error(std::move(error)) {}

	Return(Return<Type>&& other) : value(other.value), error(std::move(other.error)) {}
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
std::unique_ptr<Error> CreateErrorImplementation(const char* function, const char* file, unsigned line, const char* message, ...);