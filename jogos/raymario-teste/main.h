#ifndef MAIN_H
#define MAIN_H

#include "include/raylib.h"

static const double GLOBAL_GRAVITY = 1;

static const double PLAYER_BASE_WALK_SPEED = 5;
static const double PLAYER_BASE_JUMP_SPEED = -15;

static const double TILE_WIDTH = 40;

#define STAGE_WIDTH 50
#define STAGE_HEIGHT 15

typedef enum {
    COLLISION_TYPE_NONE,
    COLLISION_TYPE_LEFT,
    COLLISION_TYPE_RIGHT,
    COLLISION_TYPE_TOP,
    COLLISION_TYPE_BOTTOM
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
    Tile terrain[STAGE_HEIGHT][STAGE_WIDTH];
} Stage;

typedef struct {
    Tile *tile;
    CollisionType type;
} TileCollision;

void inputAndUpdatePlayer( Player *player, Stage *stage );
void drawPlayer( Player *player );
CollisionType interceptsPlayer( Player *player, SpriteData *spriteData );

void inputAndUpdateStage( Stage *stage );
void drawStage( Stage *stage );
void parseTerrain( Stage *stage, const char* terrainData );
TileCollision interceptsStage( Stage *stage, Player *player );

void drawTile( Tile *tile );
TileCollision interceptsTyle( Tile *tile, Player *player );

// utils
CollisionType collide( const Rectangle *r1, const Rectangle *r2 );

#endif // MAIN_H