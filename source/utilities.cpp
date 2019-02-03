#include "utilities.h"

#include <string>
#include <fstream>
#include <sstream>


std::string Read(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Couldn't open file " + path + ".");

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}