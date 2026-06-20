#pragma once

#include "Chunk.h"

template<>
struct std::hash<glm::vec3> {
    std::size_t operator()(const glm::vec3& k) const {
        return std::hash<size_t>()(0.5f * (k.x + k.z) * (k.x + k.z + 1) + k.z); // pairing function
    }
};

class Terrain {
public:
    Terrain();

    std::unordered_map<glm::vec3, Chunk*>& GetChunks();
    void Update(glm::vec3 cameraPos, glm::vec3 camMoveDir);
private:
    std::vector<std::vector<Chunk*>> chunksToRender;
    std::vector<glm::vec3> chunksToLoad;
    std::unordered_map<glm::vec3, Chunk*> chunks;

    bool chunkAtPos(glm::vec3 pos);
    Chunk* getChunkAtPos(glm::vec3 pos);
    // void optimizeChunkVertexData(Chunk* chunk);
};
