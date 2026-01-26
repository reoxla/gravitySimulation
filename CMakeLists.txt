cmake_minimum_required(VERSION 3.10.0)
project(gravitySimulator VERSION 0.1.0 LANGUAGES C CXX)

add_executable(gravitySimulator main.cpp src/glm/glm.hpp src/glm/gtc/matrix_transform.hpp src/KHR/khrplatform.h src/glad.c src/buffers.cpp src/shaders.cpp shaders/shader.vs shaders/shader.fs)
target_link_libraries(gravitySimulator glfw)
target_include_directories(gravitySimulator PRIVATE src)
target_include_directories(gravitySimulator PUBLIC include)
