#include "Terrain.h"

#include <iostream>

#include "Constants.h"

Terrain::Terrain() {
    // for (int i = 0; i < RENDER_DIST; ++i) {
    //     std::vector<Chunk*> chunksWithPosX;
    //     for (int j = 0; j < RENDER_DIST; ++j) {
    //         Chunk* chunk = new Chunk();
    //         chunk->Position = {CHUNK_SIZE * i, 0, CHUNK_SIZE * j};
    //         chunksWithPosX.push_back(chunk);
    //     }
    //     chunksToRender.push_back(chunksWithPosX);
    // }
    //
    // for (int i = 0; i < RENDER_DIST; ++i) {
    //     for (int j = 0; j < RENDER_DIST; ++j) {
    //         for (int l = 0; l < CHUNK_SIZE+1; ++l) {
    //             for (int m = 0; m < CHUNK_SIZE+1; ++m) {
    //                 if (j < RENDER_DIST-1) {
    //                     if (chunksToRender[i][j+1]->GetBlocks()[l][m][1].Type == Grass && chunksToRender[i][j]->GetBlocks()[l][m][CHUNK_SIZE].Type == Grass) {
    //                         chunksToRender[i][j]->GetBlocks()[l][m][CHUNK_SIZE].FrontVisible = false;
    //                         chunksToRender[i][j+1]->GetBlocks()[l][m][1].BackVisible = false;
    //                         chunksToRender[i][j+1]->GenerateChunk();
    //                         chunksToRender[i][j]->GenerateChunk();
    //                     }
    //                 }
    //
    //                 for (int n = 1; n < CHUNK_SIZE+1; ++n) {
    //                     if (i < RENDER_DIST-1) {
    //                         if (chunksToRender[i+1][j]->GetBlocks()[1][m][n].Type == Grass && chunksToRender[i][j]->GetBlocks()[CHUNK_SIZE][m][n].Type == Grass) {
    //                             chunksToRender[i][j]->GetBlocks()[CHUNK_SIZE][m][n].RightVisible = false;
    //                             chunksToRender[i+1][j]->GetBlocks()[1][m][n].LeftVisible = false;
    //                             chunksToRender[i][j]->GenerateChunk();
    //                             chunksToRender[i+1][j]->GenerateChunk();
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    //
    // for (auto& it : chunksToRender) {
    //     for (Chunk* c : it) {
    //         chunks.insert({c->Position, c});
    //     }
    // }
}

std::unordered_map<glm::vec3, Chunk*>& Terrain::GetChunks() {
    return chunks;
}

void Terrain::Update(glm::vec3 cameraPos, glm::vec3 camMoveDir) {
    int camGridPosX = glm::floor(cameraPos.x / CHUNK_SIZE);
    int camGridPosZ = glm::floor(cameraPos.z / CHUNK_SIZE);\

    int k = RENDER_DIST;
    for (int i = -k; i <= k; ++i) {
        for (int j = -k; j <= k; ++j) {
            glm::vec3 pos = {(camGridPosX + i) * CHUNK_SIZE, 0, (camGridPosZ + j) * CHUNK_SIZE};
            if (std::find(chunksToLoad.begin(), chunksToLoad.end(), pos) == chunksToLoad.end()) {
                chunksToLoad.push_back(pos);
            }
        }
    }

    for (glm::vec3& pos : chunksToLoad) {
        if (chunkAtPos(pos)) continue;

        Chunk* chunk = new Chunk(pos);

        // glm::vec3 left = pos - glm::vec3(CHUNK_SIZE, 0.f, 0.f);
        // glm::vec3 right = pos + glm::vec3(CHUNK_SIZE, 0.f, 0.f);
        // glm::vec3 back = pos - glm::vec3(0.f, 0.f, CHUNK_SIZE);
        // glm::vec3 front = pos + glm::vec3(0.f, 0.f, CHUNK_SIZE);
        //
        // std::unordered_map<glm::vec3, Chunk*>::iterator it;

        // if ((it = chunks.find(left)) != chunks.end()) {
        //     for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
        //         for (int k = 1; k < CHUNK_SIZE + 1; ++k) {
        //             if (chunk->GetBlocks()[1][j][k].Type == Grass && it->second->GetBlocks()[CHUNK_SIZE][j][k].Type == Grass) {
        //                 chunk->GetBlocks()[1][j][k].LeftVisible = false;
        //                 it->second->GetBlocks()[CHUNK_SIZE][j][k].RightVisible = false;
        //                 chunk->GenerateChunk();
        //                 it->second->GenerateChunk();
        //             }
        //         }
        //     }
        // }

        // if ((it = chunks.find(right)) != chunks.end()) {
        //     for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
        //         for (int k = 1; k < CHUNK_SIZE + 1; ++k) {
        //             if (chunk->GetBlocks()[CHUNK_SIZE][j][k].Type == Grass && it->second->GetBlocks()[1][j][k].Type == Grass) {
        //                 chunk->GetBlocks()[CHUNK_SIZE][j][k].RightVisible = false;
        //                 it->second->GetBlocks()[1][j][k].LeftVisible = false;
        //                 chunk->GenerateChunk();
        //                 it->second->GenerateChunk();
        //             }
        //         }
        //     }
        // }

        // if ((it = chunks.find(back)) != chunks.end()) {
        //     for (int i = 1; i < CHUNK_SIZE + 1; ++i) {
        //         for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
        //             if (chunk->GetBlocks()[i][j][1].Type == Grass && it->second->GetBlocks()[i][j][CHUNK_SIZE].Type == Grass) {
        //                 chunk->GetBlocks()[i][j][1].BackVisible = false;
        //                 it->second->GetBlocks()[i][j][CHUNK_SIZE].FrontVisible = false;
        //                 chunk->GenerateChunk();
        //                 it->second->GenerateChunk();
        //             }
        //         }
        //     }
        // }

        // if ((it = chunks.find(front)) != chunks.end()) {
        //     for (int i = 1; i < CHUNK_SIZE + 1; ++i) {
        //         for (int j = 1; j < CHUNK_SIZE + 1; ++j) {
        //             if (chunk->GetBlocks()[i][j][CHUNK_SIZE].Type == Grass && it->second->GetBlocks()[i][j][1].Type == Grass) {
        //                 chunk->GetBlocks()[i][j][CHUNK_SIZE].FrontVisible = false;
        //                 it->second->GetBlocks()[i][j][1].BackVisible = false;
        //                 chunk->GenerateChunk();
        //                 it->second->GenerateChunk();
        //             }
        //         }
        //     }
        // }

        if (!chunks.insert({pos, chunk}).second) {
            std::cout << "Attempt to insert an already existing chunk!!!\n";
        }
    }

    for (auto it = chunks.begin(); it != chunks.end();) {
        bool found = false;

        for (glm::vec3& pos : chunksToLoad) {
            if (it->first == pos) {
                found = true;
            }
        }

        if (found) {
            ++it;
            continue;
        }

        delete it->second;
        it = chunks.erase(it);
    }

    chunksToLoad.clear();
}

bool Terrain::chunkAtPos(glm::vec3 pos) {
    for (auto it : chunks)
        if (it.second->Position == pos)
            return true;

    return false;
}

Chunk* Terrain::getChunkAtPos(glm::vec3 pos) {
    auto it = chunks.find(pos);
    if (it != chunks.end()) return it->second;

    return nullptr;
}