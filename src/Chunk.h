#pragma once

#include <glm/gtc/matrix_transform.hpp>

static constexpr int CHUNK_SIZE = 32;

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

    void GenerateAndAddVertices(const Block& block);
    std::vector<float>& GetVertices();
    int GetNumFaces();

private:
    Block blocks[CHUNK_SIZE+2][CHUNK_SIZE+2][CHUNK_SIZE+2]{};
    std::vector<float> vertices;
    int numFaces;
};
