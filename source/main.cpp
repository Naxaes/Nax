#include <utility>

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <imgui.h>
#include <imgui_demo.cpp>
#include <implementation/imgui_impl_opengl3.h>
#include <implementation/imgui_impl_glfw.h>
#include <stb_image.h>

#include "opengl.h"
#include "shader.h"
#include "utilities.h"
#include "vao.h"
#include "loader.h"
#include "event.h"
#include "debug.h"


#if _WIN32 || _WIN64
    const char PATH_TO_VERTEX[]   = "..\\resources\\shaders\\basic.vertex.glsl";
    const char PATH_TO_FRAGMENT[] = "..\\resources\\shaders\\basic.fragment.glsl";
    const char PATH_TO_BUNNY[]    = "..\\resources\\models\\bunny.obj";
#else
    const char PATH_TO_VERTEX[]   = "../resources/shaders/basic.vertex.glsl";
    const char PATH_TO_FRAGMENT[] = "../resources/shaders/basic.fragment.glsl";
    const char PATH_TO_BUNNY[]    = "../resources/models/bunny.obj";
#endif


static EventQueue event_queue;


void OnFileDrop(GLFWwindow* window, int file_count, const char** paths)
{
    for (unsigned i = 0; i < file_count; ++i)
        AddEvent(event_queue, new FileDrop(paths[i]));
}


void OnResize(GLFWwindow* window, int width, int height)
{
    AddEvent(event_queue, new Resize(width, height));
}


struct Transform
{
    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale    = {1.0f, 1.0f, 1.0f};
};


glm::mat4 ModelMatrix(Transform transform)
{
    constexpr glm::vec3 x_axis = {1.0f, 0.0f, 0.0f};
    constexpr glm::vec3 y_axis = {0.0f, 1.0f, 0.0f};
    constexpr glm::vec3 z_axis = {0.0f, 0.0f, 1.0f};

    glm::mat4 model_matrix = {};
    model_matrix = glm::translate(model_matrix, transform.position);
    model_matrix = glm::rotate(model_matrix, transform.rotation.x, x_axis);
    model_matrix = glm::rotate(model_matrix, transform.rotation.y, y_axis);
    model_matrix = glm::rotate(model_matrix, transform.rotation.z, z_axis);
    model_matrix = glm::scale(model_matrix, transform.scale);

    return model_matrix;
}


struct SunLight
{
    glm::vec3 direction;
    glm::vec3 color;
};


struct Material
{
    glm::vec3 color = {0.2f, 0.5f, 0.8f};
    float ambient_factor  = 0.1f;
    float diffuse_factor  = 0.5f;
    float specular_factor = 0.5f;
    float shininess = 32.0f;
};

struct Window
{
    GLFWwindow* handle;
    unsigned width;
    unsigned height;
};


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

    return {window, width, height};
}


void InitializeImGui(GLFWwindow* window)
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Setup style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();
}


ShaderProgram LoadShaders(std::string vertex_path, std::string fragment_path)
{
    auto vertex_source   = Check(Read(vertex_path));
    auto fragment_source = Check(Read(fragment_path));

    // TODO(ted): Remove.
    std::cout << vertex_source   << std::endl;
    std::cout << fragment_source << std::endl;

    Shader vertex       = CreateShader(vertex_source,   ShaderType::VERTEX,   "basicv");
    Shader fragment     = CreateShader(fragment_source, ShaderType::FRAGMENT, "basicf");
    ShaderProgram basic = CreateShaderProgram({vertex, fragment}, {"position"}, "basic");

    // TODO(ted): Remove.
    auto program_info = GetShaderProgramInfo(basic);
    auto shader_info  = GetShaderInfo(program_info.shaders[0]);
    
    std::cout << "Shader '" << program_info.name << "' has attribute '" << program_info.attributes[0].name
              << "'. It has source for the vertex shader '" << shader_info.name
              << "', which is:\n" << shader_info.source << std::endl;

    std::cout << "It also has '" << program_info.uniforms[0].name << "' as a uniform." << std::endl;

    return basic;
}




int main()
{
    Window window = CreateWindow(700, 700, "Nax");
    bool should_resize = true;

    // ---- INITIALIZE GLAD ----
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);


    // ---- IMGUI SETUP ----
    InitializeImGui(window.handle);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    
    // ---- SHADER SETUP ----
    ShaderProgram basic = LoadShaders(PATH_TO_VERTEX, PATH_TO_FRAGMENT);
    

    // ---- MODEL SETUP ----
    auto  source = Check(Read(PATH_TO_BUNNY));
    auto  data   = Parse(source);
    Model model  = IndexedModel(data.first, data.second);


    // ---- DATA SETUP ----
    Transform model_transform {};

    glm::vec3 view_position  (0.0f, 1.0f,  5.0f);
    glm::vec3 view_front     (0.0f, 0.0f, -1.0f);
    glm::vec3 view_up        (0.0f, 1.0f,  0.0f);
    glm::vec3 view_velocity  (0.0f, 0.0f,  0.0f);
    float view_angle = 0.0f;


    glm::mat4 view_matrix = glm::lookAt(view_position, view_position + view_front, view_up);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(70.0f), static_cast<float>(window.width)/static_cast<float>(window.height), 0.1f, 100.0f);
    glm::vec3 clear_color (0.0f, 0.0f, 0.0f);

    SunLight sunlight;
    sunlight.direction = {1.0f, 1.0f, -1.0f};
    sunlight.color     = {1.0f, 1.0f,  1.0f};

    Material material;
    material.color = {0.2f, 0.5f, 0.8f};
    material.ambient_factor  = 0.1f;
    material.diffuse_factor  = 0.5f;
    material.specular_factor = 0.5f;
    material.shininess = 32.0f;

    Enable(basic);
    auto model_location      = CacheUniform(basic, "model");
    auto view_location       = CacheUniform(basic, "view");
    auto projection_location = CacheUniform(basic, "projection");
    auto color_location      = CacheUniform(basic, "color");
    auto sunlight_location   = CacheUniform(basic, "sunlight_direction");
    auto ambient_location    = CacheUniform(basic, "ambient_factor");
    auto diffuse_location    = CacheUniform(basic, "diffuse_factor");
    auto specular_location   = CacheUniform(basic, "specular_factor");
    auto shininess_location  = CacheUniform(basic, "shininess");


    // ---- GAME LOOP ----
    while (!glfwWindowShouldClose(window.handle))
    {
        // const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        // ---- EVENT HANDLING ----
        for (auto& event : GetAll(event_queue))
        {
            if (event->type == Event::FILE_DROP)
            {
                // This cast should always be safe.
                // TODO(ted): This should probably be loaded in a different thread.
                auto file_drop = reinterpret_cast<FileDrop*>(event);
                auto source = Read(file_drop->path);
                if (source.error)
                {
                    Print(*source.error);
                }
                else
                {
                    data  = Parse(source.value);
                    model = IndexedModel(data.first, data.second);
                }
            }
            else if (event->type == Event::RESIZE)
            {
                // This cast should always be safe.
                auto resize_event = reinterpret_cast<Resize*>(event);
                window.width  = resize_event->width;
                window.height = resize_event->height;
                should_resize = true;
            }
        }
        Clear(event_queue);

        if (!io.WantCaptureKeyboard)
        {
            glm::vec3 right = glm::normalize(glm::cross(view_front, view_up));
            constexpr float speed = 0.1f;

            // Position
            if (glfwGetKey(window.handle, GLFW_KEY_D))             view_position += right * speed;
            if (glfwGetKey(window.handle, GLFW_KEY_A))             view_position -= right * speed;
            if (glfwGetKey(window.handle, GLFW_KEY_W))             view_position += view_front * speed;
            if (glfwGetKey(window.handle, GLFW_KEY_S))             view_position -= view_front * speed;
            if (glfwGetKey(window.handle, GLFW_KEY_SPACE))         view_position += view_up * speed;
            if (glfwGetKey(window.handle, GLFW_KEY_LEFT_CONTROL))  view_position -= view_up * speed;

            // Direction
            if (glfwGetKey(window.handle, GLFW_KEY_E))
            {
                view_angle += speed * 0.2f;
                view_front.z = -glm::cos(view_angle);
                view_front.x =  glm::sin(view_angle);
            }
            if (glfwGetKey(window.handle, GLFW_KEY_Q))
            {
                view_angle -= speed * 0.2f;
                view_front.z = -glm::cos(view_angle);
                view_front.x =  glm::sin(view_angle);
            }
        }


        // ---- IMGUI RENDERING ----
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGuiWindowFlags window_flags = 0;
            // window.handle_flags |= ImGuiwindow.handleFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoScrollbar;
            window_flags |= ImGuiWindowFlags_MenuBar;
            window_flags |= ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoResize;
            // window.handle_flags |= ImGuiwindow.handleFlags_NoCollapse;
            // window.handle_flags |= ImGuiwindow.handleFlags_NoNav;
            // window.handle_flags |= ImGuiwindow.handleFlags_AlwaysAutoResize;

            if (ImGui::Begin("Options", nullptr, window_flags))   // Passing 'bool* p_open' displays a Close button on the upper-right corner of the window.handle, the pointed value will be set to false when the button is pressed.
            {
                ImGui::Columns(2);
                ImGui::SetWindowPos(ImVec2(0, 0));
                ImGui::SetWindowSize(ImVec2(window.width, 0));

                ImGui::ColorEdit3("Background color", &clear_color.x);
                ImGui::SliderFloat3("Light direction", &sunlight.direction.x, -1.0f, 1.0f);
                sunlight.direction = glm::normalize(sunlight.direction);  // Always make sure directions are normalized.
                ImGui::SliderFloat("Ambient factor",  &material.ambient_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Diffuse factor",  &material.diffuse_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Specular factor", &material.specular_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Shininess", &material.shininess, 0.0f, 256.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::NextColumn();

                ImGui::Text("Transform");
                ImGui::SliderFloat3("Position", &model_transform.position.x, -3.0f,  3.0f);
                ImGui::SliderFloat3("Rotation", &model_transform.rotation.x, -3.14f, 3.14f);
                ImGui::SliderFloat3("Scale",    &model_transform.scale.x,     0.0f,  2.0f);
                if (ImGui::Button("Reset"))
                {
                    model_transform.position = glm::vec3(0.0f, 0.0f, -2.0f);
                    model_transform.rotation = glm::vec3(0.0f, 0.0f,  0.0f);
                    model_transform.scale    = glm::vec3(1.0f, 1.0f,  1.0f);
                }

            }
            ImGui::End();
        }
        {
            // ImGui::ShowDemowindow.handle();
        }

        // ---- UPDATING ----
        // This should be cached and not done at every tick.

        view_matrix = glm::lookAt(view_position, view_position + view_front, view_up);
        projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(window.width) / static_cast<float>(window.height), 0.1f, 100.0f);

        Enable(basic);
        SetUniform(model_location,      ModelMatrix(model_transform));
        SetUniform(view_location,       view_matrix);
        SetUniform(projection_location, projection_matrix);
        SetUniform(color_location,      material.color);
        SetUniform(sunlight_location,   sunlight.direction);
        SetUniform(ambient_location,    material.ambient_factor);
        SetUniform(diffuse_location,    material.diffuse_factor);
        SetUniform(specular_location,   material.specular_factor);
        SetUniform(shininess_location,  material.shininess);


        // ---- USER RENDERING ----
        if (should_resize)
        {
            should_resize = false;
            GLCALL(glViewport(0, 0, window.width, window.height));
        }

        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f));

        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glEnable(GL_DEPTH_TEST));

        Enable(basic);

        GLCALL(glBindVertexArray(model.vao));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo));
        GLCALL(glDrawElements(GL_TRIANGLES, model.count, GL_UNSIGNED_INT, nullptr));


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window.handle);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window.handle);
    glfwTerminate();

    return 0;
}