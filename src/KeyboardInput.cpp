#include "KeyboardInput.h"

void KeyboardInput::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    for (int i = 0; i < 350; ++i) {
        if (key == i && action == GLFW_PRESS) {
            GetInstance().keys_pressed[i] = true;
            GetInstance().keys_clicked[i] = true;
        }

        if (key == i && action == GLFW_RELEASE) {
            GetInstance().keys_pressed[i] = false;
        }
    }
}

void KeyboardInput::EndFrame() {
    for (bool &key : GetInstance().keys_clicked) {
        key = false;
    }
}

bool KeyboardInput::KeyPressed(unsigned int key) {
    return GetInstance().keys_pressed[key];
}
