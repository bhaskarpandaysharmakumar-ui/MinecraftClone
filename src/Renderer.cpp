#include "Renderer.h"

#include <glad/glad.h>

Renderer::Renderer() { }

void Renderer::AddChunk(Chunk &chunk) {
    ChunkRenderData data;

    float faceTexCoord[] = {
        0.f, 0.f,
        0.f, 1.f,
        1.f, 0.f,
        1.f, 0.f,
        0.f, 1.f,
        1.f, 1.f,
    };
    std::vector<float> texCoords;
    for (int j = 0; j < chunk.GetNumFaces(); ++j) {
        for (int i = 0; i < sizeof(faceTexCoord)/sizeof(float); ++i) {
            texCoords.push_back(faceTexCoord[i]);
        }
    }

    glGenVertexArrays(1, &data.Vao);
    glBindVertexArray(data.Vao);

    glGenBuffers(1, &data.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, data.vbo);
    glBufferData(GL_ARRAY_BUFFER, chunk.GetVertices().size() * sizeof(float), chunk.GetVertices().data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &data.tbo);
    glBindBuffer(GL_ARRAY_BUFFER, data.tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    chunks.insert({data, chunk});
}

void Renderer::Render(Shader& shader) {
    for (auto it = chunks.begin(); it != chunks.end(); ++it) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, it->second.Position);
        shader.SetMat4("model", model);

        glBindVertexArray(it->first.Vao);
        glDrawArrays(GL_TRIANGLES, 0, it->second.GetVertices().size()/3);
        glBindVertexArray(0);
    }
}
