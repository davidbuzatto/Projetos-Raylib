#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "global_constants.h"
#include "base.h"
#include "player.h"
#include "stage.h"

void inputPlayer( Player *player, Stage *stage ) {

    SpriteData *playerD = &player->data;

    // jump
    if ( IsKeyPressed( KEY_SPACE ) && player->state == PLAYER_STATE_ON_GROUND ) {
        playerD->vy = PLAYER_BASE_JUMP_SPEED;
        player->state = PLAYER_STATE_JUMPING;
    }

    // walk
    if ( IsKeyDown( KEY_RIGHT ) ) {
        playerD->vx = PLAYER_BASE_WALK_SPEED;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        playerD->vx = -PLAYER_BASE_WALK_SPEED;
    } else {
        playerD->vx = 0;
    }

}

void updatePlayer( Player *player, Stage *stage ) {

    SpriteData *playerD = &player->data;
    SpriteData *stageD = &stage->data;

    playerD->x += playerD->vx;
    playerD->y += playerD->vy;

    if ( playerD->y + playerD->height >= stageD->y ) {
        playerD->y = stageD->y - playerD->height;
        playerD->vy = 0;
        player->state = PLAYER_STATE_ON_GROUND;
    }

    playerD->vy += GLOBAL_GRAVITY;

}

void drawPlayer( Player *player ) {

    SpriteData *data = &(player->data);

    DrawRectangle( (int) data->x, (int) data->y,
        (int) data->width, (int) data->height, 
        data->baseColor );

}

CollisionResult interceptsPlayer( Player *player, SpriteData *spriteData ) {
    return COLLISION_NONE;
}