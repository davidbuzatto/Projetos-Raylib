#ifndef PLAYER_H
#define PLAYER_H

#include "base.h"
#include "stage.h"

static const double PLAYER_BASE_WALK_SPEED = 5;
static const double PLAYER_BASE_JUMP_SPEED = -15;

typedef enum {
    PLAYER_STATE_ON_GROUND,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_DOWN,
    PLAYER_STATE_DYING
} PlayerState;

typedef struct {
    SpriteData data;
    PlayerState state;
} Player;

void inputPlayer( Player *player, Stage *stage );
void updatePlayer( Player *player, Stage *stage );
void drawPlayer( Player *player );
CollisionType interceptsPlayer( Player *player, SpriteData *spriteData );

#endif // PLAYER_H