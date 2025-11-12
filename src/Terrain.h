#pragma once

#include "Chunk.h"
#include "Constants.h"

class Terrain {
public:
    Terrain();

    Chunk* (&GetChunks())[RENDER_DIST][RENDER_DIST];
private:
    Chunk* chunks[RENDER_DIST][RENDER_DIST]{};
};
