#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "core/Shader.h"

class Camera {
public:
    glm::vec3 Position;
    float Sensitivity, Speed;

    Camera(float fov=77.f, float aspect=16.f/9.f, float near=0.1f, float far=1000.f);
    void Update(const Shader &shader, float dt);

private:
    glm::mat4 proj;
    float yaw, pitch;
};
