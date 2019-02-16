#pragma once

#include "vao.h"

std::vector<std::string> Split(std::string source, char delimiter);
std::pair<std::vector<Vertex>, std::vector<GLuint>> Parse(std::string source);