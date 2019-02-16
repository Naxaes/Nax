cmake_minimum_required(VERSION 3.2)
project(Nax)

set(CMAKE_CXX_STANDARD 14)

# ---- LIBRARIES ----

# GLAD
add_library(glad libraries/glad/source/glad.c)
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
add_executable(Nax source/main.cpp source/opengl.cpp source/utilities.cpp source/shader.cpp source/vao.cpp source/loader.cpp)
target_include_directories(Nax PRIVATE include/)

target_link_libraries(Nax glad glfw assimp imgui)
target_include_directories(Nax PRIVATE libraries/stb/)
target_include_directories(Nax PRIVATE libraries/glm/)
target_include_directories(Nax PRIVATE libraries/glad/include/)
target_include_directories(Nax PRIVATE libraries/assimp-4.1.0/include/)
target_include_directories(Nax PRIVATE libraries/imgui/)

# ---- Tests ----
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(unit-test tests/unit-tests/loader_test.cpp source/loader.cpp)
target_link_libraries(unit-test glad glfw assimp imgui gtest)
target_include_directories(unit-test PRIVATE include/)
target_include_directories(unit-test PRIVATE libraries/stb/)
target_include_directories(unit-test PRIVATE libraries/glm/)
target_include_directories(unit-test PRIVATE libraries/glad/include/)
target_include_directories(unit-test PRIVATE libraries/assimp-4.1.0/include/)
target_include_directories(unit-test PRIVATE libraries/imgui/)
target_include_directories(unit-test PRIVATE libraries/googletest/googletest/include)
target_include_directories(unit-test PRIVATE libraries/googletest/googlemock/include)

add_test(NAME loader-test COMMAND unit-test)