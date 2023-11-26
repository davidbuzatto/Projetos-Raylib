#ifndef MAIN_H
#define MAIN_H

#include "include/raylib.h"

static const double GLOBAL_GRAVITY = 1;

static const double PLAYER_BASE_WALK_SPEED = 5;
static const double PLAYER_BASE_JUMP_SPEED = -15;

static const double TILE_WIDTH = 40;

typedef enum {
    COLLISION_NONE,
    COLLISION_LEFT,
    COLLISION_RIGHT,
    COLLISION_TOP,
    COLLISION_BOTTOM
} CollisionType;

typedef enum {
    PLAYER_STATE_ON_GROUND,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_DOWN,
    PLAYER_STATE_DYING
} PlayerState;

typedef struct {
    double x;
    double y;
    double width;
    double height;
    double vx;
    double vy;
    double angle;
    Color baseColor;
} SpriteData;

typedef struct {
    SpriteData data;
    PlayerState state;
} Player;
typedef struct {
    SpriteData data;
    bool collideable;
    bool visible;
} Tile;
typedef struct {
    SpriteData data;
    Tile terrain[15][20];
} Stage;

typedef struct {
    Tile *tile;
    CollisionType type;
} TileCollision;

void inputPlayer( Player *player, Stage *stage );
void updatePlayer( Player *player, Stage *stage );
void drawPlayer( Player *player );
CollisionType interceptsPlayer( Player *player, SpriteData *spriteData );

void updateStage( Stage *stage );
void drawStage( Stage *stage );
void parseTerrain( Stage *stage, const char* terrainData );
TileCollision interceptsStage( Stage *stage, Player *player );

void drawTile( Tile *tile );
TileCollision interceptsTyle( Tile *tile, Player *player );

#endif // MAIN_H