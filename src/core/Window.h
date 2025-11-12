#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();

    bool ShouldNotClose();
    void Clear();
    void EndFrame();
    void Terminate();
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* window = nullptr;
};
