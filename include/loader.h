#pragma once

#include <string>
#include <vector>

#include "vao.h"

TexturedModel LoadModel(const std::string& path);

std::vector<std::string> Split(std::string source, char delimiter);
std::pair<std::vector<Vertex>, std::vector<GLuint>> Parse(std::string source);