#include <iostream>

#include <stbi/stb_image.h>

#include "core/Window.h"
#include "core/Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Terrain.h"
#include "input/KeyboardInput.h"

int main() {
    Window window;
    Terrain terrain;
    Shader shader{"../res/shaders/shader.vs", "../res/shaders/shader.fs"};
    Renderer renderer;

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
    // camera.Position = {CHUNK_SIZE * (RENDER_DIST - 1) * 0.5f, 8, CHUNK_SIZE * (RENDER_DIST - 1) * 0.5f};
    camera.Position.y = 8.f;

    float dt = 0.f;
    float previousTime = (float) glfwGetTime();

    glm::vec3 prevCamPos(0, 0, 0);

    bool wireframeMode = false;

    float tickTime = 0.f;
    float tickTimeElapsed = 0.f;

    while(window.ShouldNotClose())
    {
        window.Clear();

        float currentTime = (float) glfwGetTime();
        dt = currentTime - previousTime;

        tickTimeElapsed += dt;

        if (KeyboardInput::KeyClicked(GLFW_KEY_TAB)) {
            wireframeMode = !wireframeMode;
        }

        wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, texture);

        camera.Update(shader, dt);

        // std::cout << dt << '\n';

        glm::vec3 camMoveDir = camera.Position - prevCamPos;
        prevCamPos = camera.Position;

        if (tickTimeElapsed >= tickTime) {
            terrain.Update(camera.Position, camMoveDir);
            tickTimeElapsed = 0.f;
        }
        // std::cout << camMoveDir.x << ", " << camMoveDir.z << "\n";

        // for (auto it = terrain.GetChunks().begin(); it != terrain.GetChunks().end(); ++it) {
        //     for (Chunk* chunk : *it) {
        //         renderer.AddChunk(chunk);
        //     }
        // }

        for (auto& it : terrain.GetChunks()) {
            renderer.AddChunk(it.second);
        }

        renderer.Render(shader);

        shader.Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);

        previousTime = currentTime;

        window.EndFrame();
    }

    window.Terminate();

    return 0;
}
