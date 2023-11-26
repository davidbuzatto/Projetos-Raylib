#ifndef TILE_H
#define TILE_H

#include "base.h"

static const double TILE_WIDTH = 40;

typedef struct {
    SpriteData data;
    bool collideable;
    bool visible;
} Tile;

void drawTile( Tile *tile );

#endif // TILE_H