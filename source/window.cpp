#include "window.h"

#include <GLFW/glfw3.h>

#include "event.h"
#include "debug.h"

EventQueue event_queue;


void OnFileDrop(GLFWwindow* window, int file_count, const char** paths)
{
    for (unsigned i = 0; i < file_count; ++i)
        AddEvent(event_queue, new FileDrop(paths[i]));
}

void OnResize(GLFWwindow* window, int width, int height)
{
    AddEvent(event_queue, new Resize(width, height));
}

static void OnMouseMovement(GLFWwindow* window, double x, double y)
{
    AddEvent(event_queue, new MouseMovement(x, y));
}

void OnMouseClick(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        AddEvent(event_queue, new MouseClick(MouseClick::Button::LEFT, MouseClick::Event::CLICKED));
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        AddEvent(event_queue, new MouseClick(MouseClick::Button::LEFT, MouseClick::Event::RELEASED));
}

Window CreateWindow(unsigned width, unsigned height, std::string title)
{
    // ---- INITIALIZE GLFW ----
    Assert(glfwInit(), "Couldn't initialize GLFW.");

    // ---- WINDOW HINTS ----
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GLFW_TRUE);


    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    Assert(window, "Couldn't create window.");

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetDropCallback(window, OnFileDrop);
    glfwSetFramebufferSizeCallback(window, OnResize);
    glfwSetMouseButtonCallback(window, OnMouseClick);
    glfwSetCursorPosCallback(window, OnMouseMovement);


    return {window, width, height};
}