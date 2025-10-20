#include "Chunk.h"

#include <iostream>

Chunk::Chunk() : numBlocks(0) {
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            for (int k = 0; k < CHUNK_SIZE; ++k) {
                int x = (int) (2 * glm::sin(i));
                int z = (int) (2 * glm::sin(k));
                std::cout << x * z << "\n";
                blocks[i][j][k] = {{i, j, k}, Grass, true};
            }
        }
    }

    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            for (int k = 0; k < CHUNK_SIZE; ++k) {
                if (i > 0 && i < CHUNK_SIZE-1 && j > 0 && j < CHUNK_SIZE-1 && k > 0 && k < CHUNK_SIZE-1) {
                    if (!(blocks[i+1][j][k].Type == Grass && blocks[i-1][j][k].Type == Grass &&
                        blocks[i][j][k+1].Type == Grass && blocks[i][j][k-1].Type == Grass &&
                        blocks[i][j+1][k].Type == Grass && blocks[i][j-1][k].Type == Grass)) {

                        Block& block = blocks[i][j][k];
                        block.Visible = true;
                        block.Type = Grass;
                        block.Position = {i, j, k};
                        GenerateAndAddVertices(block);
                        numBlocks++;
                    }
                } else {
                    Block& block = blocks[i][j][k];
                    block.Visible = true;
                    block.Type = Grass;
                    block.Position = {i, j, k};
                    GenerateAndAddVertices(block);
                    numBlocks++;
                }
            }
        }
    }
}

void Chunk::GenerateAndAddVertices(const Block& block) {
    float vertices[] = {
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
    };
    for (int i = 0; i < 36*3; i+=3) {
        glm::vec4 ndcPos = {vertices[i], vertices[i+1], vertices[i+2], 1};
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, block.Position);
        glm::vec4 transformedPos = model * ndcPos;
        this->vertices.push_back(transformedPos.x);
        this->vertices.push_back(transformedPos.y);
        this->vertices.push_back(transformedPos.z);
    }
}

std::vector<float>& Chunk::GetVertices() {
    return vertices;
}

int Chunk::GetNumBlocks() {
    return numBlocks;
}
