#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

enum BlockType {
    Air, Grass
};

struct Block {
    glm::vec3 Position;
    BlockType Type = Air;
    bool FrontVisible = true;
    bool BackVisible = true;
    bool LeftVisible = true;
    bool RightVisible = true;
    bool TopVisible = true;
    bool BottomVisible = true;
};

class Chunk {
public:
    Chunk();

    glm::vec3 Position;

    std::vector<float>& GetVertices();
    void GenerateChunk();
    Block (&GetBlocks())[CHUNK_SIZE+2][CHUNK_SIZE+2][CHUNK_SIZE+2];

    std::vector<float> vertices;

private:
    Block blocks[CHUNK_SIZE+2][CHUNK_SIZE+2][CHUNK_SIZE+2]{};

    void GenerateVertices(const Block& block);
};
