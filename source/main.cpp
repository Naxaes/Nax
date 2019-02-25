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
    unsigned width = 700, height = 700;
    bool should_resize = true;
    window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad. */
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glfwSetDropCallback(window, OnFileDrop);
    glfwSetFramebufferSizeCallback(window, OnResize);


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
    auto vertex_source   = Read(PATH_TO_VERTEX);
    auto fragment_source = Check(Read(PATH_TO_FRAGMENT));

    if (vertex_source.error)    Print(*vertex_source.error);
    // if (fragment_source.error)  Print(*fragment_source.error);

    if (vertex_source.error /* || fragment_source.error */ ) return -1;

    std::cout << vertex_source.value   << std::endl;
    std::cout << fragment_source << std::endl;

    Shader vertex       = CreateShader(vertex_source.value,   ShaderType::VERTEX,   "basicv");
    Shader fragment     = CreateShader(fragment_source,       ShaderType::FRAGMENT, "basicf");
    ShaderProgram basic = CreateShaderProgram({vertex, fragment}, {"position"}, "basic");

    std::cout << "Shader '" << basic.info->name << "' has attribute '" << basic.info->attributes[0].name
              << "'. It has source for the vertex shader '" << basic.info->shaders[0].info->name
              << "', which is:\n" << basic.info->shaders[0].info->source << std::endl;

    std::cout << "It also has '" << basic.info->uniforms[0].name << "' as a uniform." << std::endl;

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

    auto  source = Check(Read(PATH_TO_BUNNY));
    auto  data   = Parse(source);
    Model model  = IndexedModel(data.first, data.second);



    // ---- DATA SETUP ----
    glm::vec3 model_position (0.0f, 0.0f, 0.0f);
    glm::vec3 model_rotation (0.0f, 0.0f, 0.0f);
    glm::vec3 model_scale    (1.0f, 1.0f, 1.0f);
    glm::mat4 model_matrix = {};
    model_matrix = glm::translate(model_matrix, model_position);

    glm::vec3 view_position  (0.0f, 1.0f,  5.0f);
    glm::vec3 view_front     (0.0f, 0.0f, -1.0f);
    glm::vec3 view_up        (0.0f, 1.0f,  0.0f);
    glm::vec3 view_velocity  (0.0f, 0.0f,  0.0f);
    float view_angle = 0.0f;

    glm::mat4 view_matrix = glm::lookAt(view_position, view_position + view_front, view_up);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(1.0f), static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.0f);
    glm::vec3 clear_color (0.0f, 0.0f, 0.0f);
    glm::vec3 model_color (0.2f, 0.5f, 0.8f);
    glm::vec3 sunlight_direction (1.0f, 1.0f, -1.0f);
    float ambient_factor  = 0.1f;
    float diffuse_factor  = 0.5f;
    float specular_factor = 0.5f;
    float shininess = 32.0f;

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
    while (!glfwWindowShouldClose(window))
    {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

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
                    data = Parse(source.value);
                    model = IndexedModel(data.first, data.second);
                }
            }
            else if (event->type == Event::RESIZE)
            {
                // This cast should always be safe.
                auto resize_event = reinterpret_cast<Resize*>(event);
                width  = resize_event->width;
                height = resize_event->height;
                should_resize = true;
            }
        }
        Clear(event_queue);

        if (!io.WantCaptureKeyboard)
        {
            glm::vec3 right = glm::normalize(glm::cross(view_front, view_up));
            constexpr float speed = 0.1f;

            // Position
            if (glfwGetKey(window, GLFW_KEY_D))             view_position += right * speed;
            if (glfwGetKey(window, GLFW_KEY_A))             view_position -= right * speed;
            if (glfwGetKey(window, GLFW_KEY_W))             view_position += view_front * speed;
            if (glfwGetKey(window, GLFW_KEY_S))             view_position -= view_front * speed;
            if (glfwGetKey(window, GLFW_KEY_SPACE))         view_position += view_up * speed;
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL))  view_position -= view_up * speed;

            // Direction
            if (glfwGetKey(window, GLFW_KEY_E))
            {
                view_angle += speed * 0.2f;
                view_front.z = -glm::cos(view_angle);
                view_front.x =  glm::sin(view_angle);
            }
            if (glfwGetKey(window, GLFW_KEY_Q))
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
            // window_flags |= ImGuiWindowFlags_NoTitleBar;
            window_flags |= ImGuiWindowFlags_NoScrollbar;
            window_flags |= ImGuiWindowFlags_MenuBar;
            window_flags |= ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoResize;
            // window_flags |= ImGuiWindowFlags_NoCollapse;
            // window_flags |= ImGuiWindowFlags_NoNav;
            // window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

            if (ImGui::Begin("Options", nullptr, window_flags))   // Passing 'bool* p_open' displays a Close button on the upper-right corner of the window, the pointed value will be set to false when the button is pressed.
            {
                ImGui::Columns(2);
                ImGui::SetWindowPos(ImVec2(0, 0));
                ImGui::SetWindowSize(ImVec2(width, height * 0.33f));

                ImGui::ColorEdit3("Background color", &clear_color.x);
                ImGui::SliderFloat3("Light direction", &sunlight_direction.x, -1.0f, 1.0f);
                sunlight_direction = glm::normalize(sunlight_direction);  // Always make sure directions are normalized.
                ImGui::SliderFloat("Ambient factor",  &ambient_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Diffuse factor",  &diffuse_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Specular factor", &specular_factor, 0.0f, 1.0f);
                ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

                ImGui::NextColumn();

                ImGui::Text("Transform");
                ImGui::SliderFloat3("Position", &model_position.x, -3.0f,  3.0f);
                ImGui::SliderFloat3("Rotation", &model_rotation.x, -3.14f, 3.14f);
                ImGui::SliderFloat3("Scale",    &model_scale.x,     0.0f,  2.0f);
                if (ImGui::Button("Reset"))
                {
                    model_position = glm::vec3(0.0f, 0.0f, -2.0f);
                    model_rotation = glm::vec3(0.0f, 0.0f,  0.0f);
                    model_scale    = glm::vec3(1.0f, 1.0f,  1.0f);
                }

            }
            ImGui::End();
        }
        {
            // ImGui::ShowDemoWindow();
        }

        // ---- UPDATING ----
        // This should be cached and not done at every tick.

        model_matrix = {};
        model_matrix = glm::translate(model_matrix, model_position);
        model_matrix = glm::rotate(model_matrix, model_rotation.x, {1.0f, 0.0f, 0.0f});
        model_matrix = glm::rotate(model_matrix, model_rotation.y, {0.0f, 1.0f, 0.0f});
        model_matrix = glm::rotate(model_matrix, model_rotation.z, {0.0f, 0.0f, 1.0f});
        model_matrix = glm::scale(model_matrix, model_scale);

        view_matrix = glm::lookAt(view_position, view_position + view_front, view_up);
        projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

        Enable(basic);
        SetUniform(model_location,      model_matrix);
        SetUniform(view_location,       view_matrix);
        SetUniform(projection_location, projection_matrix);
        SetUniform(color_location,      model_color);
        SetUniform(sunlight_location,   sunlight_direction);
        SetUniform(ambient_location,    ambient_factor);
        SetUniform(diffuse_location,    diffuse_factor);
        SetUniform(specular_location,   specular_factor);
        SetUniform(shininess_location,  shininess);


        // ---- USER RENDERING ----
        if (should_resize)
        {
            should_resize = false;
            GLCALL(glViewport(0, 0, width, height));
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