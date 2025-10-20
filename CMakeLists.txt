cmake_minimum_required(VERSION 3.31)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 20)

add_executable(MinecraftClone src/main.cpp libraries/glad.c
        src/Shader.cpp
        src/Shader.h
        src/MouseInput.h
        src/MouseInput.cpp
        libraries/include/stbi/stb_image.h
        libraries/stb_image.cpp
        src/Camera.cpp
        src/Camera.h
        src/KeyboardInput.cpp
        src/KeyboardInput.h
        src/Chunk.cpp
        src/Chunk.h)

target_include_directories(MinecraftClone PRIVATE ${CMAKE_SOURCE_DIR}/libraries/include)

target_link_libraries(MinecraftClone ${CMAKE_SOURCE_DIR}/libraries/libs/libglfw3.a)