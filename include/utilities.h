#pragma once

#include <string>

#include "errors.h"

extern const char DIRECTORY_SEPERATOR;

Return<std::string> Read(const std::string& path);