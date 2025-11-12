#include "Terrain.h"

Terrain::Terrain() {
    for (int i = 0; i < RENDER_DIST; ++i) {
        for (int j = 0; j < RENDER_DIST; ++j) {
            chunks[i][j] = new Chunk();
            chunks[i][j]->Position = {CHUNK_SIZE * (i - (RENDER_DIST-1)/2), 0, CHUNK_SIZE * (j - (RENDER_DIST-1)/2)};
        }
    }

    for (int i = 0; i < RENDER_DIST; ++i) {
        for (int j = 0; j < RENDER_DIST; ++j) {
            for (int l = 0; l < CHUNK_SIZE+1; ++l) {
                for (int m = 0; m < CHUNK_SIZE+1; ++m) {
                    if (j < RENDER_DIST-1) {
                        if (chunks[i][j+1]->GetBlocks()[l][m][1].Type == Grass && chunks[i][j]->GetBlocks()[l][m][CHUNK_SIZE].Type == Grass) {
                            chunks[i][j]->GetBlocks()[l][m][CHUNK_SIZE].FrontVisible = false;
                            chunks[i][j+1]->GetBlocks()[l][m][1].BackVisible = false;
                            chunks[i][j+1]->GenerateChunk();
                            chunks[i][j]->GenerateChunk();
                        }
                    }
                    if (j > 0) {
                        if (chunks[i][j-1]->GetBlocks()[l][m][CHUNK_SIZE].Type == Grass && chunks[i][j]->GetBlocks()[l][m][1].Type == Grass) {
                            chunks[i][j]->GetBlocks()[l][m][1].BackVisible = false;
                            chunks[i][j-1]->GetBlocks()[l][m][CHUNK_SIZE].FrontVisible = false;
                            chunks[i][j-1]->GenerateChunk();
                            chunks[i][j]->GenerateChunk();
                        }
                    }

                    for (int n = 1; n < CHUNK_SIZE+1; ++n) {
                        if (i > 0) {
                            if (chunks[i-1][j]->GetBlocks()[CHUNK_SIZE][m][n].Type == Grass && chunks[i][j]->GetBlocks()[1][m][n].Type == Grass) {
                                chunks[i][j]->GetBlocks()[1][m][n].LeftVisible = false;
                                chunks[i-1][j]->GetBlocks()[CHUNK_SIZE][m][n].RightVisible = false;
                                chunks[i][j]->GenerateChunk();
                                chunks[i-1][j]->GenerateChunk();
                            }
                        }
                        if (i < RENDER_DIST-1) {
                            if (chunks[i+1][j]->GetBlocks()[1][m][n].Type == Grass && chunks[i][j]->GetBlocks()[CHUNK_SIZE][m][n].Type == Grass) {
                                chunks[i][j]->GetBlocks()[CHUNK_SIZE][m][n].RightVisible = false;
                                chunks[i+1][j]->GetBlocks()[1][m][n].LeftVisible = false;
                                chunks[i][j]->GenerateChunk();
                                chunks[i+1][j]->GenerateChunk();
                            }
                        }
                    }
                }
            }
        }
    }
}

Chunk* (&Terrain::GetChunks())[RENDER_DIST][RENDER_DIST] {
    return chunks;
}
