#pragma once

#include <unordered_map>

#include "Chunk.h"
#include "core/Shader.h"

struct ChunkRenderData {
    unsigned int Vao, vbo, tbo;

    bool operator==(const ChunkRenderData& other) const {
        return this->Vao == other.Vao;
    }
};

template<>
struct std::hash<ChunkRenderData> {
    std::size_t operator()(const ChunkRenderData& k) const {
        return std::hash<unsigned int>()(k.Vao);
    }
};

class Renderer {
public:
    Renderer();

    void AddChunk(Chunk* chunk);
    void Render(Shader& shader);

private:
    std::unordered_map<ChunkRenderData, Chunk*> chunks;
};
