#include "Chunk.h"

#include <iostream>

Chunk::Chunk() : numFaces(0) {
    for (int i = 1; i < CHUNK_SIZE+1; ++i) {
        for (int j = 1; j < CHUNK_SIZE+1; ++j) {
            for (int k = 1; k < CHUNK_SIZE+1; ++k) {
                blocks[i][j][k].Position = {i, j, k};

                if (j > 5) {
                    float f = glm::sqrt(i+k+j*k);
                    int x = (int) glm::abs((f+3) * glm::pow(glm::sin(f+3), 2) * glm::pow(glm::cos(f+2.5), 3));
                    blocks[i][5+x][k].Type = Grass;
                    numFaces += 6;
                }
                else {
                    blocks[i][j][k].Type = Grass;
                    numFaces += 6;
                }
            }
        }
    }

    for (int i = 1; i < CHUNK_SIZE+1; ++i) {
        for (int j = 1; j < CHUNK_SIZE+1; ++j) {
            for (int k = 1; k < CHUNK_SIZE+1; ++k) {
                if (blocks[i][j][k].Type == Air) continue;

                if (blocks[i-1][j][k].Type == Grass) {
                    blocks[i][j][k].LeftVisible = false;
                    numFaces--;
                }
                if (blocks[i+1][j][k].Type == Grass) {
                    blocks[i][j][k].RightVisible = false;
                    numFaces--;
                }
                if (blocks[i][j+1][k].Type == Grass) {
                    blocks[i][j][k].TopVisible = false;
                    numFaces--;
                }
                if (blocks[i][j-1][k].Type == Grass) {
                    blocks[i][j][k].BottomVisible = false;
                    numFaces--;
                }
                if (blocks[i][j][k-1].Type == Grass) {
                    blocks[i][j][k].BackVisible = false;
                    numFaces--;
                }
                if (blocks[i][j][k+1].Type == Grass) {
                    blocks[i][j][k].FrontVisible = false;
                    numFaces--;
                }
                GenerateAndAddVertices(blocks[i][j][k]);
            }
        }
    }
}

void Chunk::GenerateAndAddVertices(const Block& block) {
    float data[] = {
        // back
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,

        // front
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,

        // right
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,

        // left
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,

        // top
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        // bottom
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
    };
    std::vector<float> vertices;
    if (block.BackVisible)
        for (int i = 0; i <= 17; ++i)
            vertices.push_back(data[i]);
    if (block.FrontVisible)
        for (int i = 18; i <= 17 + 18; ++i)
            vertices.push_back(data[i]);
    if (block.RightVisible)
        for (int i = 18*2; i <= 17 + 18*2; ++i)
            vertices.push_back(data[i]);
    if (block.LeftVisible)
        for (int i = 18*3; i <= 17 + 18*3; ++i)
            vertices.push_back(data[i]);
    if (block.TopVisible)
        for (int i = 18*4; i <= 17 + 18*4; ++i)
            vertices.push_back(data[i]);
    if (block.BottomVisible)
        for (int i = 18*5; i <= 17 + 18*5; ++i)
            vertices.push_back(data[i]);

    for (int i = 0; i < vertices.size(); i+=3) {
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

int Chunk::GetNumFaces() {
    return numFaces;
}
