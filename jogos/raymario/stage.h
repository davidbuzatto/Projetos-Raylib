#ifndef STAGE_H
#define STAGE_H

#include "base.h"
#include "tile.h"

typedef struct {
    SpriteData data;
    Tile terrain[15][20];
} Stage;

void updateStage( Stage *stage );
void drawStage( Stage *stage );
void parseTerrain( Stage *stage, const char* terrainData );

#endif // STAGE_H