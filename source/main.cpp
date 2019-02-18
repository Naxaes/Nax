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
#include <implementation/imgui_impl_opengl3.h>
#include <implementation/imgui_impl_glfw.h>
#include <stb_image.h>

#include "opengl.h"
#include "shader.h"
#include "utilities.h"
#include "vao.h"
#include "loader.h"


struct Event
{
    enum Type { FILE_DROP };
    Type type;
};

struct FileDrop : public Event
{
    std::string path;
    FileDrop(std::string path) : path(std::move(path)) { type = Event::FILE_DROP; }
};

std::vector<Event*> event_queue;

void OnFileDrop(GLFWwindow* window, int file_count, const char** paths)
{
    for (unsigned i = 0; i < file_count; ++i)
        event_queue.push_back(new FileDrop(paths[i]));
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
    unsigned width = 640, height = 480;
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
    std::cout << vertex_source   << std::endl;
    std::cout << fragment_source << std::endl;

    Shader vertex       = CreateShader(vertex_source,   ShaderType::VERTEX,   "basicv");
    Shader fragment     = CreateShader(fragment_source, ShaderType::FRAGMENT, "basicf");
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
    Model quad  = IndexedModel(vertices, indices);
    auto  data  = Parse(Read("../resources/models/bunny.obj"));
    Model model = IndexedModel(data.first, data.second);

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


    // ---- DATA SETUP ----
    glm::vec3 model_position (0.0f, 0.0f, 0.0f);
    glm::vec3 model_rotation (0.0f, 0.0f, 0.0f);
    glm::vec3 model_scale    (1.0f, 1.0f, 1.0f);
    glm::mat4 model_matrix = {};
    model_matrix = glm::translate(model_matrix, model_position);

    glm::vec3 view_position  (0.0f, 1.0f,  5.0f);
    glm::vec3 view_direction (0.0f, 0.0f, -1.0f);
    glm::vec3 view_up (0.0f, 1.0f,  0.0f);

    glm::mat4 view_matrix = glm::lookAt(view_position, view_position + view_direction, view_up);
    glm::mat4 projection_matrix = glm::perspective(glm::radians(1.0f), 45.0f, 0.1f, 100.0f);
    glm::vec3 clear_color (0.0f, 0.0f, 0.0f);
    glm::vec3 model_color (0.2f, 0.5f, 0.8f);
    glm::vec3 sunlight_direction (1.0f, 1.0f, -1.0f);
    float ambient_factor  = 0.1f;
    float diffuse_factor  = 0.5f;
    float specular_factor = 0.5f;
    float shininess = 32.0f;

    GLCALL(glUseProgram(basic.id));
    GLCALL(GLint model_location      = glGetUniformLocation(basic.id, "model"));
    GLCALL(GLint view_location       = glGetUniformLocation(basic.id, "view"));
    GLCALL(GLint projection_location = glGetUniformLocation(basic.id, "projection"));
    GLCALL(GLint color_location      = glGetUniformLocation(basic.id, "color"));
    GLCALL(GLint sunlight_location   = glGetUniformLocation(basic.id, "sunlight_direction"));
    GLCALL(GLint ambient_location    = glGetUniformLocation(basic.id, "ambient_factor"));
    GLCALL(GLint diffuse_location    = glGetUniformLocation(basic.id, "diffuse_factor"));
    GLCALL(GLint specular_location   = glGetUniformLocation(basic.id, "specular_factor"));
    GLCALL(GLint shininess_location  = glGetUniformLocation(basic.id, "shininess"));


    // ---- GAME LOOP ----
    while (!glfwWindowShouldClose(window))
    {
        // ---- EVENT HANDLING ----
        for (auto event : event_queue)
        {
            if (event->type == Event::FILE_DROP)
            {
                // This cast should always be safe.
                // TODO(ted): This should probably be loaded in a different thread.
                auto file_drop = reinterpret_cast<FileDrop*>(event);
                try
                {
                    data  = Parse(Read(file_drop->path));
                    model = IndexedModel(data.first, data.second);
                }
                catch (const std::runtime_error& error)
                {
                    std::cerr << error.what() << std::endl;
                }
            }
        }
        event_queue.clear();


        // ---- IMGUI RENDERING ----
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Light");
            ImGui::ColorEdit3("Background color", &clear_color.x);
            ImGui::SliderFloat3("Light direction", &sunlight_direction.x, -1.0f, 1.0f);
            sunlight_direction = glm::normalize(sunlight_direction);  // Always make sure directions are normalized.
            ImGui::SliderFloat("Ambient factor",  &ambient_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Diffuse factor",  &diffuse_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Specular factor", &specular_factor, 0.0f, 1.0f);
            ImGui::SliderFloat("Shininess", &shininess, 0.0f, 256.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        {
            ImGui::Begin("Model");
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
            ImGui::End();
        }
        {
            ImGui::Begin("View");
            ImGui::SliderFloat3("Position",  &view_position.x,  -10.0f, 10.0f);
            ImGui::SliderFloat2("Direction", &view_direction.x, - 1.0f,  1.0f);
            view_direction = glm::normalize(view_direction);
            if (ImGui::Button("Reset"))
            {
                view_position  = glm::vec3(0.0f, 0.0f, 5.0f);
                view_direction = glm::vec3(0.0f, 0.0f, -1.0f);
            }
            ImGui::End();
        }

        // ---- UPDATING ----
        // This should be cached and not done at every tick.
        GLCALL(glUseProgram(basic.id));

        model_matrix = {};
        model_matrix = glm::translate(model_matrix, model_position);
        model_matrix = glm::rotate(model_matrix, model_rotation.x, {1.0f, 0.0f, 0.0f});
        model_matrix = glm::rotate(model_matrix, model_rotation.y, {0.0f, 1.0f, 0.0f});
        model_matrix = glm::rotate(model_matrix, model_rotation.z, {0.0f, 0.0f, 1.0f});
        model_matrix = glm::scale(model_matrix, model_scale);

        view_matrix = glm::lookAt(view_position, view_position + view_direction, view_up);
        projection_matrix = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

        GLCALL(glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model_matrix)));
        GLCALL(glUniformMatrix4fv(view_location,  1, GL_FALSE, glm::value_ptr(view_matrix)));
        GLCALL(glUniformMatrix4fv(projection_location,  1, GL_FALSE, glm::value_ptr(projection_matrix)));
        GLCALL(glUniform3f(color_location, model_color.x, model_color.y, model_color.z));
        GLCALL(glUniform3f(sunlight_location, sunlight_direction.x, sunlight_direction.y, sunlight_direction.z));
        GLCALL(glUniform1f(ambient_location,  ambient_factor));
        GLCALL(glUniform1f(diffuse_location,  diffuse_factor));
        GLCALL(glUniform1f(specular_location, specular_factor));
        GLCALL(glUniform1f(shininess_location, shininess));


        // ---- USER RENDERING ----
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        GLCALL(glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f));

        GLCALL(glEnable(GL_CULL_FACE));
        GLCALL(glEnable(GL_DEPTH_TEST));

        GLCALL(glUseProgram(basic.id));

        // GLCALL(glBindVertexArray(quad.vao));
        // GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo));
        // GLCALL(glDrawElements(GL_TRIANGLES, quad.count, GL_UNSIGNED_INT, nullptr));

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