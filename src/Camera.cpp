#include "Camera.h"

#include "MouseInput.h"
#include "KeyboardInput.h"

Camera::Camera(float fov, float aspect, float near, float far) :
    Position(0, 0, 3), Sensitivity(0.08f), Speed(4.f), yaw(0.f), pitch(0.f) {
    proj = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::Update(const Shader &shader, float dt) {
    pitch -= MouseInput::GetOffsetY() * Sensitivity;
    yaw += MouseInput::GetOffsetX() * Sensitivity;

    auto view = glm::mat4(1.f);
    view = glm::rotate(view, glm::radians(pitch), {1, 0, 0});
    view = glm::rotate(view, glm::radians(yaw), {0, 1, 0});
    view = glm::translate(view, -Position);

    glm::vec3 forward = {view[0][2], view[1][2], view[2][2]};
    glm::vec3 left = {view[0][0], view[1][0], view[2][0]};
    glm::vec3 up = {view[0][1], view[1][1], view[2][1]};

    if (KeyboardInput::KeyPressed(GLFW_KEY_W))
        Position -= Speed * forward * dt;
    if (KeyboardInput::KeyPressed(GLFW_KEY_S))
        Position += Speed * forward * dt;
    if (KeyboardInput::KeyPressed(GLFW_KEY_A))
        Position -= Speed * left * dt;
    if (KeyboardInput::KeyPressed(GLFW_KEY_D))
        Position += Speed * left * dt;
    if (KeyboardInput::KeyPressed(GLFW_KEY_E))
        Position += Speed * up * dt;
    if (KeyboardInput::KeyPressed(GLFW_KEY_Q))
        Position -= Speed * up * dt;

    shader.SetMat4("view", view);
    shader.SetMat4("proj", proj);
}
