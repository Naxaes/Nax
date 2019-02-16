#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <imgui.h>
#include <implementation/imgui_impl_opengl3.h>
#include <implementation/imgui_impl_glfw.h>
#include <stb_image.h>

#include "opengl.h"
#include "shader.h"
#include "utilities.h"
#include "vao.h"
#include "loader.h"


int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad. */
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);


    // ---- IMGUI SETUP ----
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Setup style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // ---- SHADER SETUP ----
    std::string vertex_source   = Read("../resources/shaders/basic.vertex.glsl");
    std::string fragment_source = Read("../resources/shaders/basic.fragment.glsl");
    std::cout << vertex_source << std::endl;
    std::cout << fragment_source << std::endl;

    Shader vertex       = CreateShader(vertex_source,   ShaderType::VERTEX,   "basicv");
    Shader fragment     = CreateShader(fragment_source, ShaderType::FRAGMENT, "basicf");
    ShaderProgram basic = CreateShaderProgram({vertex, fragment}, {"position"}, "basic");

    std::cout << "Shader '" << basic.info->name << "' has attribute '" << basic.info->attributes[0].name
              << "'. It has source for the vertex shader '" << basic.info->shaders[0].info->name
              << "', which is:\n" << basic.info->shaders[0].info->source << std::endl;


    // ---- MODEL SETUP ----
    std::vector<Vertex> vertices = {
            //    Positions          Texture coordinates           Normals
            {{-0.5f, -0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Left bottom
            {{ 0.5f,  0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Right top
            {{-0.5f,  0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Left  top
            {{ 0.5f, -0.5f, 0.0f},       {0.0f, 0.0f},        {0.0f, 0.0f, 0.0f}},    // Right bottom
    };
    std::vector<GLuint> indices = {
            0, 1, 2,
            0, 3, 1
    };
    Model quad  = IndexedModel(vertices, indices);
    auto data   = Parse(Read("../resources/models/bunny.obj"));
    Model bunny = IndexedModel(data.first, data.second);

    // std::string source =
    //         "# A simple quad\n"
    //         "v -0.5, -0.5, 0.0\n"
    //         "v 0.5, 0.5, 0.0\n"
    //         "v -0.5, 0.5, 0.0\n"
    //         "v 0.5, -0.5, 0.0\n"
    //         "vn 0.0, 0.0, 0.0\n"
    //         "vn 0.0, 0.0, 0.0\n"
    //         "vn 0.0, 0.0, 0.0\n"
    //         "vn 0.0, 0.0, 0.0\n"
    //         "f 1//1 2//2 3//3\n"
    //         "f 1//1 4//4 2//2\n";
    // auto data = Parse(source);
    // Model quad = IndexedModel(data.first, data.second);


    // ---- GAME LOOP ----
    float clear_color[4] = {0, 0, 0, 0};
    while (!glfwWindowShouldClose(window))
    {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            // static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);         // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // Rendering
        GLCALL(glClear(GL_COLOR_BUFFER_BIT));
        GLCALL(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));

        GLCALL(glUseProgram(basic.id));

        // GLCALL(glBindVertexArray(quad.vao));
        // GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo));
        // GLCALL(glDrawElements(GL_TRIANGLES, quad.count, GL_UNSIGNED_INT, nullptr));

        GLCALL(glBindVertexArray(bunny.vao));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunny.ebo));
        GLCALL(glDrawElements(GL_TRIANGLES, bunny.count, GL_UNSIGNED_INT, nullptr));


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}