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

    for (int i = 0; i < RENDER_DIST; ++i) {
        for (int j = 0; j < RENDER_DIST; ++j) {
            renderer.AddChunk(terrain.GetChunks()[i][j]);
        }
    }

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
    camera.Position.y = 8;

    float dt = 0.f;
    float previousTime = (float) glfwGetTime();

    bool wireframeMode = false;

    while(window.ShouldNotClose())
    {
        window.Clear();

        float currentTime = (float) glfwGetTime();
        dt = currentTime - previousTime;

        if (KeyboardInput::KeyClicked(GLFW_KEY_TAB)) {
            wireframeMode = !wireframeMode;
        }

        wireframeMode ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, texture);

        camera.Update(shader, dt);

        renderer.Render(shader);

        shader.Unbind();
        glBindTexture(GL_TEXTURE_2D, 0);

        previousTime = currentTime;

        window.EndFrame();
    }

    window.Terminate();

    return 0;
}
