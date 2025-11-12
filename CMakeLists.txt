cmake_minimum_required(VERSION 3.31)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 20)

add_executable(MinecraftClone src/main.cpp libraries/glad.c
        src/core/Shader.cpp
        src/core/Shader.h
        src/input/MouseInput.h
        src/input/MouseInput.cpp
        libraries/include/stbi/stb_image.h
        libraries/stb_image.cpp
        src/Camera.cpp
        src/Camera.h
        src/input/KeyboardInput.cpp
        src/input/KeyboardInput.h
        src/Chunk.cpp
        src/Chunk.h
        src/Renderer.cpp
        src/Renderer.h
        src/Terrain.cpp
        src/Terrain.h
        src/core/Window.cpp
        src/core/Window.h
        src/Constants.h)

target_include_directories(MinecraftClone PRIVATE ${CMAKE_SOURCE_DIR}/libraries/include ${CMAKE_SOURCE_DIR}/src/)

target_link_libraries(MinecraftClone ${CMAKE_SOURCE_DIR}/libraries/libs/libglfw3.a)