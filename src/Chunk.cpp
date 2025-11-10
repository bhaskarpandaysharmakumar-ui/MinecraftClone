#include "Chunk.h"

#include <iostream>

double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
double lerp(double t, double a, double b) { return a + t * (b - a); }

double grad(int hash, double x, double y, double z) {
    int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
    double u = h < 8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
            v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

int p[512], permutation[] = {
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

double noise(double x, double y, double z) {
    int X = (int) glm::floor(x) & 255, // FIND UNIT CUBE THAT
            Y = (int) glm::floor(y) & 255, // CONTAINS POINT.
            Z = (int) glm::floor(z) & 255;
    x -= glm::floor(x); // FIND RELATIVE X,Y,Z
    y -= glm::floor(y); // OF POINT IN CUBE.
    z -= glm::floor(z);
    double u = fade(x), // COMPUTE FADE CURVES
            v = fade(y), // FOR EACH OF X,Y,Z.
            w = fade(z);
    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z, // HASH COORDINATES OF
            B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z; // THE 8 CUBE CORNERS,

    return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), // AND ADD
                                grad(p[BA], x - 1, y, z)), // BLENDED
                        lerp(u, grad(p[AB], x, y - 1, z), // RESULTS
                             grad(p[BB], x - 1, y - 1, z))), // FROM  8
                lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1), // CORNERS
                             grad(p[BA + 1], x - 1, y, z - 1)), // OF CUBE
                     lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                          grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

Chunk::Chunk() : numFaces(0), Position(0, 8, 0) {
    for (int i = 0; i < 25; ++i) {
        p[256 + i] = p[i] = permutation[i];
    }

    for (int i = 1; i < CHUNK_SIZE + 1; ++i) {
        for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
            for (int k = 1; k < CHUNK_SIZE + 1; ++k) {
                blocks[i][j][k].Position = {i, j, k};

                double y = 20 * noise((i+2)/64.f, (j +10)/32.f, (k+20)/32.f);
                if ((int)abs(y) > 0) {
                    blocks[i][(int)glm::abs(y)][k].Type = Grass;
                    numFaces += 6;
                }
            }
        }
    }

    for (int i = 1; i < CHUNK_SIZE + 1; ++i) {
        for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
            for (int k = 1; k < CHUNK_SIZE + 1; ++k) {
                if (blocks[i][j][k].Type == Air) continue;

                if (blocks[i - 1][j][k].Type == Grass) {
                    blocks[i][j][k].LeftVisible = false;
                    numFaces--;
                }
                if (blocks[i + 1][j][k].Type == Grass) {
                    blocks[i][j][k].RightVisible = false;
                    numFaces--;
                }
                if (blocks[i][j + 1][k].Type == Grass) {
                    blocks[i][j][k].TopVisible = false;
                    numFaces--;
                }
                if (blocks[i][j - 1][k].Type == Grass) {
                    blocks[i][j][k].BottomVisible = false;
                    numFaces--;
                }
                if (blocks[i][j][k - 1].Type == Grass) {
                    blocks[i][j][k].BackVisible = false;
                    numFaces--;
                }
                if (blocks[i][j][k + 1].Type == Grass) {
                    blocks[i][j][k].FrontVisible = false;
                    numFaces--;
                }
                GenerateAndAddVertices(blocks[i][j][k]);
            }
        }
    }
}

void Chunk::GenerateAndAddVertices(const Block &block) {
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
        for (int i = 18 * 2; i <= 17 + 18 * 2; ++i)
            vertices.push_back(data[i]);
    if (block.LeftVisible)
        for (int i = 18 * 3; i <= 17 + 18 * 3; ++i)
            vertices.push_back(data[i]);
    if (block.TopVisible)
        for (int i = 18 * 4; i <= 17 + 18 * 4; ++i)
            vertices.push_back(data[i]);
    if (block.BottomVisible)
        for (int i = 18 * 5; i <= 17 + 18 * 5; ++i)
            vertices.push_back(data[i]);

    for (int i = 0; i < vertices.size(); i += 3) {
        glm::vec4 ndcPos = {vertices[i], vertices[i + 1], vertices[i + 2], 1};
        glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, {block.Position.x - CHUNK_SIZE/2.f,
                                            block.Position.y,
                                            block.Position.z - CHUNK_SIZE/2.f});
        glm::vec4 transformedPos = model * ndcPos;
        this->vertices.push_back(transformedPos.x);
        this->vertices.push_back(transformedPos.y);
        this->vertices.push_back(transformedPos.z);
    }
}

std::vector<float> &Chunk::GetVertices() {
    return vertices;
}

int Chunk::GetNumFaces() {
    return numFaces;
}
