#pragma once

#include <GLFW/glfw3.h>

#include "event.h"

extern EventQueue event_queue;

struct Window
{
    GLFWwindow* handle;
    unsigned width;
    unsigned height;
};

void OnFileDrop(GLFWwindow* window, int file_count, const char** paths);
void OnResize(GLFWwindow* window, int width, int height);
static void OnMouseMovement(GLFWwindow* window, double x, double y);
void OnMouseClick(GLFWwindow* window, int button, int action, int mods);
Window CreateWindow(unsigned width, unsigned height, std::string title);