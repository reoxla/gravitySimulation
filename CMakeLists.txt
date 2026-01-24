cmake_minimum_required(VERSION 3.10.0)
project(gravitySimulator VERSION 0.1.0 LANGUAGES C CXX)

add_executable(gravitySimulator main.cpp src/glad.h src/glad.c src/glm/glm.hpp src/buffers.h src/buffers.cpp src/glm/gtc/matrix_transform.hpp)
target_link_libraries(gravitySimulator glfw)
target_include_directories(gravitySimulator PUBLIC include)
