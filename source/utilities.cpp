#include "utilities.h"

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "errors.h"

#if _WIN32 || _WIN64
	const char DIRECTORY_SEPERATOR = '\\';
#else
	const char DIRECTORY_SEPERATOR = '/';
#endif


Return<std::string> Read(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return CreateError("Couldn't open file '%s'.", path.c_str());

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}