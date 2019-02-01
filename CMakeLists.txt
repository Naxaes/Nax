cmake_minimum_required(VERSION 3.2)
project(Nax)

set(CMAKE_CXX_STANDARD 14)

# ---- LIBRARIES ----

# GLAD
add_library(glad libraries/glad/src/glad.c)
target_include_directories(glad PRIVATE libraries/glad/include/)

# GLFW
add_subdirectory(libraries/glfw-3.3.1)

# ASSIMP
add_subdirectory(libraries/assimp-4.1.0)

# IMGUI
add_library(
    imgui
    libraries/imgui/imgui.cpp
    libraries/imgui/imgui_draw.cpp
    libraries/imgui/implementation/imgui_impl_opengl3.cpp
    libraries/imgui/implementation/imgui_impl_glfw.cpp
)
target_link_libraries(imgui glad)
target_link_libraries(imgui glfw)
target_include_directories(imgui PRIVATE libraries/imgui/)
target_include_directories(imgui PRIVATE libraries/glad/include/)

# GTEST
add_subdirectory(libraries/googletest)


# ---- PROJECT SETTINGS ----
add_executable(Nax source/main.cpp)

target_link_libraries(Nax glad glfw assimp imgui gtest)
target_include_directories(Nax PRIVATE libraries/stb/)
target_include_directories(Nax PRIVATE libraries/glm/)
target_include_directories(Nax PRIVATE libraries/glad/include/)
target_include_directories(Nax PRIVATE libraries/assimp-4.1.0/include/)
target_include_directories(Nax PRIVATE libraries/imgui/)

# ---- Tests ----

# add_executable(example example.cpp)
# target_link_libraries(example gtest_main)
# add_test(NAME example_test COMMAND example)