#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"
#include "include/raymath.h"

#include "main.h"

Texture2D PLAYER_WALKING_TEXTURE_CACHE[4];
Texture2D PLAYER_JUMPING_TEXTURE_CACHE[4];

void updatePlayerBoundingBoxes( Player *player ) {

    Rectangle *r = &player->sp.rect;

    player->bbLeft = (Rectangle){
        .x = r->x,
        .y = r->y + player->bbLength,
        .width = player->bbLength,
        .height = r->height - 2 * player->bbLength
    };

    player->bbRight = (Rectangle){
        .x = r->x + r->width - player->bbLength,
        .y = r->y + player->bbLength,
        .width = player->bbLength,
        .height = r->height - 2 * player->bbLength
    };

    player->bbTop = (Rectangle){
        .x = r->x + player->bbLength,
        .y = r->y,
        .width = r->width - 2 * player->bbLength,
        .height = player->bbLength
    };

    player->bbBottom = (Rectangle){
        .x = r->x + player->bbLength,
        .y = r->y + r->height - player->bbLength,
        .width = r->width - 2 * player->bbLength,
        .height = player->bbLength
    };

}

CollisionType checkCollisionPlayerVsTile( Player *player, Tile *tile ) {

    if ( CheckCollisionRecs( player->bbLeft, tile->sp.rect ) ) {
        return COLLISION_TYPE_LEFT;
    } else if ( CheckCollisionRecs( player->bbRight, tile->sp.rect ) ) {
        return COLLISION_TYPE_RIGHT;
    }

    if ( CheckCollisionRecs( player->bbTop, tile->sp.rect ) ) {
        return COLLISION_TYPE_TOP;
    } else if ( CheckCollisionRecs( player->bbBottom, tile->sp.rect ) ) {
        return COLLISION_TYPE_BOTTOM;
    }

    return COLLISION_TYPE_NONE;

}

void resolveCollisionPlayerVsTile( Player *player, Tile *tile, CollisionType collisionType ) {

    switch ( collisionType ) {
        case COLLISION_TYPE_LEFT:
            player->sp.rect.x = tile->sp.rect.x + tile->sp.rect.width;
            break;
        case COLLISION_TYPE_RIGHT:
            player->sp.rect.x = tile->sp.rect.x - player->sp.rect.width;
            break;
        case COLLISION_TYPE_TOP:
            player->sp.rect.y = tile->sp.rect.y + tile->sp.rect.height;
            player->sp.vel.y = 0;
            player->jumping = true;
            player->onGround = false;
            break;
        case COLLISION_TYPE_BOTTOM:
            player->sp.rect.y = tile->sp.rect.y - player->sp.rect.height;
            player->sp.vel.y = 0;
            player->jumping = false;
            player->onGround = true;
            break;
        case COLLISION_TYPE_NONE:
            break;
    }

    if ( collisionType != COLLISION_TYPE_NONE ) {
        updatePlayerBoundingBoxes( player );
    }

}

void resolveCollisionPlayerVsTileMap( Player *player, TileMap *tileMap ) {

    for ( int i = 0; i < tileMap->rows; i++ ) {
        for ( int j = 0; j < tileMap->columns; j++ ) {
            Tile *tile = &tileMap->tiles[i*tileMap->columns+j];
            if ( tile != NULL && tile->collideable ) {
                CollisionType collisionType = checkCollisionPlayerVsTile( 
                    player, tile );
                if ( collisionType != COLLISION_TYPE_NONE ) {
                    resolveCollisionPlayerVsTile( player, tile, collisionType );                    
                }
            }
        }
    }

}

void drawPlayer( Player *player ) {

    /*DrawRectangle( 
        player->sp.rect.x, 
        player->sp.rect.y, 
        player->sp.rect.width, 
        player->sp.rect.height, 
        player->sp.color );*/

    if ( player->state == PLAYER_STATE_IDLE && !player->jumping ) {
        
        if ( player->lookingDirection == LOOKING_TO_THE_LEFT ) {
            DrawTexture( player->walkingTextureFrame[0], 
                player->sp.rect.x, player->sp.rect.y, WHITE );
        } else if ( player->lookingDirection == LOOKING_TO_THE_RIGHT ) {
            DrawTexture( player->walkingTextureFrame[2], 
                player->sp.rect.x, player->sp.rect.y, WHITE );
        }

    } else if ( player->onGround ) {

        player->walkingCounter++;
            
        if ( player->walkingCounter % 5 == 0 ) {
            player->walkingCounter = 0;
            player->currentWalkingFrame++;
            if ( player->currentWalkingFrame > player->maxWalkingFrame ) {
                player->currentWalkingFrame = 0;
            }
        }

        if ( player->lookingDirection == LOOKING_TO_THE_LEFT ) {

            DrawTexture( player->walkingTextureFrame[player->currentWalkingFrame], 
                player->sp.rect.x, player->sp.rect.y, WHITE );

        } else if ( player->lookingDirection == LOOKING_TO_THE_RIGHT ) {

            DrawTexture( player->walkingTextureFrame[2 + player->currentWalkingFrame], 
                player->sp.rect.x, player->sp.rect.y, WHITE );

        }

    } else if ( player->jumping ) {

        // 0: goind up, 1: going down
        int frame = player->sp.vel.y < 0 ? 0 : 1;

        if ( player->lookingDirection == LOOKING_TO_THE_LEFT ) {

            DrawTexture( player->jumpingTextureFrame[frame], 
                player->sp.rect.x, player->sp.rect.y, WHITE );

        } else if ( player->lookingDirection == LOOKING_TO_THE_RIGHT ) {

            DrawTexture( player->jumpingTextureFrame[2 + frame], 
                player->sp.rect.x, player->sp.rect.y, WHITE );

        }

    }

    /*DrawRectangleRec( player->bbLeft, GREEN );
    DrawRectangleRec( player->bbRight, ORANGE );
    DrawRectangleRec( player->bbTop, RED );
    DrawRectangleRec( player->bbBottom, VIOLET );*/

}

void loadPlayerTextureCaches( Player *player ) {

    PLAYER_WALKING_TEXTURE_CACHE[0] = LoadTexture( "resources/images/sprites/mario1L.png" );
    PLAYER_WALKING_TEXTURE_CACHE[1] = LoadTexture( "resources/images/sprites/mario2L.png" );
    PLAYER_WALKING_TEXTURE_CACHE[2] = LoadTexture( "resources/images/sprites/mario1R.png" );
    PLAYER_WALKING_TEXTURE_CACHE[3] = LoadTexture( "resources/images/sprites/mario2R.png" );

    PLAYER_JUMPING_TEXTURE_CACHE[0] = LoadTexture( "resources/images/sprites/marioJump1L.png" );
    PLAYER_JUMPING_TEXTURE_CACHE[1] = LoadTexture( "resources/images/sprites/marioJump2L.png" );
    PLAYER_JUMPING_TEXTURE_CACHE[2] = LoadTexture( "resources/images/sprites/marioJump1R.png" );
    PLAYER_JUMPING_TEXTURE_CACHE[3] = LoadTexture( "resources/images/sprites/marioJump2R.png" );

    player->walkingTextureFrame[0] = PLAYER_WALKING_TEXTURE_CACHE[0];
    player->walkingTextureFrame[1] = PLAYER_WALKING_TEXTURE_CACHE[1];
    player->walkingTextureFrame[2] = PLAYER_WALKING_TEXTURE_CACHE[2];
    player->walkingTextureFrame[3] = PLAYER_WALKING_TEXTURE_CACHE[3];
    
    player->jumpingTextureFrame[0] = PLAYER_JUMPING_TEXTURE_CACHE[0];
    player->jumpingTextureFrame[1] = PLAYER_JUMPING_TEXTURE_CACHE[1];
    player->jumpingTextureFrame[2] = PLAYER_JUMPING_TEXTURE_CACHE[2];
    player->jumpingTextureFrame[3] = PLAYER_JUMPING_TEXTURE_CACHE[3];

}

void unloadPlayerTextureCaches( void ) {
    UnloadTexture( PLAYER_WALKING_TEXTURE_CACHE[0] );
    UnloadTexture( PLAYER_WALKING_TEXTURE_CACHE[1] );
    UnloadTexture( PLAYER_WALKING_TEXTURE_CACHE[2] );
    UnloadTexture( PLAYER_WALKING_TEXTURE_CACHE[3] );
    UnloadTexture( PLAYER_JUMPING_TEXTURE_CACHE[0] );
    UnloadTexture( PLAYER_JUMPING_TEXTURE_CACHE[1] );
    UnloadTexture( PLAYER_JUMPING_TEXTURE_CACHE[2] );
    UnloadTexture( PLAYER_JUMPING_TEXTURE_CACHE[3] );
}