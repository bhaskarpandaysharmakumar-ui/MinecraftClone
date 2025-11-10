#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <stbi/stb_image.h>

#include "Shader.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Camera.h"
#include "Chunk.h"
#include "Renderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Woo-Hoo", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    gladLoadGL();

    glViewport(0, 0, 1280, 720);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, MouseInput::CursorPosCallback);
    glfwSetKeyCallback(window, KeyboardInput::KeyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Chunk* chunk = new Chunk();
    Chunk* chunk2 = new Chunk();
    chunk2->Position.x = 32;

    Shader shader("../res/shaders/shader.vs", "../res/shaders/shader.fs");

    Renderer renderer;
    renderer.AddChunk(*chunk);
    renderer.AddChunk(*chunk2);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../res/textures/Grass1.png", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    Camera camera;
    camera.Position.y = 16;

    float dt = 0.f;
    float previousTime = (float) glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f/255.f, 0.f/255.f, 0.f/255.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = (float) glfwGetTime();
        dt = currentTime - previousTime;

        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, texture);

        camera.Update(shader, dt);

        renderer.Render(shader);

        shader.Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);

        previousTime = currentTime;
        MouseInput::EndFrame();
        KeyboardInput::EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}