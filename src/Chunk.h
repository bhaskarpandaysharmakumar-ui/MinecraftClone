#pragma once

#include <glm/gtc/matrix_transform.hpp>

static constexpr int CHUNK_SIZE = 8;

enum BlockType {
    Air, Grass
};

struct Block {
    glm::vec3 Position;
    BlockType Type;
    bool Visible;
};

class Chunk {
public:
    Chunk();

    void GenerateAndAddVertices(const Block& block);
    std::vector<float>& GetVertices();
    int GetNumBlocks();

private:
    void Generate();
    void AddVertexPosition();

    Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    std::vector<float> vertices;
    int numBlocks;
};
