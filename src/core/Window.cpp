#include "Window.h"

#include <iostream>

#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "Constants.h"

Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Woo-Hoo", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window!\n";
        Terminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    gladLoadGL();

    glViewport(0, 0, 1280, 720);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseInput::CursorPosCallback);
    glfwSetKeyCallback(window, KeyboardInput::KeyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

bool Window::ShouldNotClose() {
    return !glfwWindowShouldClose(window);
}

void Window::Clear() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f/255.f, 0.f/255.f, 0.f/255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::EndFrame() {
    MouseInput::EndFrame();
    KeyboardInput::EndFrame();
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::Terminate() {
    glfwTerminate();
}

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
