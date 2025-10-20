#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stbi/stb_image.h>

#include "Shader.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Camera.h"
#include "Chunk.h"

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

    float vertices[] = {
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f
    };
    unsigned int indices[] = {
        0, 1, 3,
        3, 1, 2,
        4, 5, 7,
        7, 5, 6,
        8, 9, 11,
        11, 9, 10,
        12, 13, 15,
        15, 13, 14,
        16, 17, 19,
        19, 17, 18,
        20, 21, 23,
        23, 21, 22
    };
    float baseTexCoors[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,

        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
    };

    Chunk chunk;

    std::vector<float> texCoords;
    for (int j = 0; j < chunk.GetNumBlocks(); ++j) {
        for (int i = 0; i < sizeof(baseTexCoors)/sizeof(float); ++i) {
            texCoords.push_back(baseTexCoors[i]);
        }
    }

    unsigned int vbo, vao, ebo, tbo;
    Shader shader("../res/shaders/shader.vs", "../res/shaders/shader.fs");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, chunk.GetVertices().size() * sizeof(float), chunk.GetVertices().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    /*glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../res/textures/Grass.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    Camera camera;

    float dt = 0.f;
    float previousTime = (float) glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = (float) glfwGetTime();
        dt = currentTime - previousTime;

        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, texture);

        //auto model = glm::mat4(1.0f);

        camera.Update(shader, dt);

        //shader.SetMat4("model", model);


        glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, chunk.GetVertices().size()/3);
        glBindVertexArray(0);

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