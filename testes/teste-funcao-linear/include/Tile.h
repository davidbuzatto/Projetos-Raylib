#pragma once
#include "raylib.h"

typedef enum TileType {
    TILE_TYPE_SLOPE_UP,
    TILE_TYPE_SLOPE_DOWN
} TileType;

typedef enum CollisionType {
    COLLISION_TYPE_NONE = 0,
    COLLISION_TYPE_UP,
    COLLISION_TYPE_DOWN
} CollisionType;

typedef struct Tile {
    Vector2 pos;
    Vector2 dim;
    TileType type;
    Color color;
    CollisionType collisionType;
} Tile;

void drawTile( Tile *tile );
CollisionType checkCollisionTileVector2( Tile *tile, Vector2 pos );