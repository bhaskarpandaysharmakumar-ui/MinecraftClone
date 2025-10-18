#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "MouseInput.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
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

    unsigned int vbo, vao, ebo;
    Shader shader("../res/shaders/shader.vs", "../res/shaders/shader.fs");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glm::mat4 proj = glm::perspective(glm::radians(77.f), 16.f / 9.f, 0.1f, 1000.f);

    glm::vec3 camPos = {0, 0, 3};
    float sensitivity = 0.08f;
    float speed = 4.f;
    float pitch = 0.f, yaw = 0.f;

    float dt = 0.f;
    float previousTime = (float) glfwGetTime();

    // TODO: why does multiplying dt to pitch and yaw make mouse input frame rate "dependent"
    while(!glfwWindowShouldClose(window))
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentTime = (float) glfwGetTime();
        dt = currentTime - previousTime;

        shader.Bind();

        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-45.f), {1, 0, 0});

        pitch -= MouseInput::GetOffsetY() * sensitivity;
        yaw += MouseInput::GetOffsetX() * sensitivity;

        auto view = glm::mat4(1.f);
        view = glm::rotate(view, glm::radians(pitch), {1, 0, 0});
        view = glm::rotate(view, glm::radians(yaw), {0, 1, 0});
        view = glm::translate(view, -camPos);

        glm::vec3 forward = {view[0][2], view[1][2], view[2][2]};
        glm::vec3 left = {view[0][0], view[1][0], view[2][0]};
        glm::vec3 up = {view[0][1], view[1][1], view[2][1]};

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camPos -= speed * forward * dt;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camPos += speed * forward * dt;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camPos -= speed * left * dt;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camPos += speed * left * dt;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camPos += speed * up * dt;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camPos -= speed * up * dt;

        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("proj", proj);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        shader.Unbind();

        MouseInput::EndFrame();
        previousTime = currentTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}