#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "main.h"

void inputAndUpdatePlayer( Player *player, Stage *stage ) {

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
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        //playerD->vy = PLAYER_BASE_WALK_SPEED;
    } else if ( IsKeyDown( KEY_UP ) ) {
        //playerD->vy = -PLAYER_BASE_WALK_SPEED;
    } else {
        playerD->vx = 0;
        //playerD->vy = 0;
    }

    playerD->x += playerD->vx;
    playerD->y += playerD->vy;

    TileCollision tc = interceptsStage( stage, player );
    Tile *tile = tc.tile;

    switch ( tc.type ) {
        case COLLISION_TYPE_LEFT:
            playerD->x = tile->data.x - playerD->width - 1;
            //printf( "left\n" );
            break;
        case COLLISION_TYPE_RIGHT:
            playerD->x = tile->data.x + tile->data.width + 1;
            //printf( "right\n" );
            break;
        case COLLISION_TYPE_TOP:
            playerD->y = tile->data.y - playerD->height - 1;
            playerD->vy = 0;
            player->state = PLAYER_STATE_ON_GROUND;
            //printf( "top\n" );
            break;
        case COLLISION_TYPE_BOTTOM:
            playerD->y = tile->data.y + tile->data.height + 1;
            playerD->vy = 5;
            //printf( "bottom\n" );
            break;
        default: 
            break;
    }

    playerD->vy += GLOBAL_GRAVITY;

}

void drawPlayer( Player *player ) {

    SpriteData *data = &player->data;

    DrawRectangle( (int) data->x, (int) data->y,
                   (int) data->width, (int) data->height, 
                   data->baseColor );

}

CollisionType interceptsPlayer( Player *player, SpriteData *spriteData ) {
    return COLLISION_TYPE_NONE;
}