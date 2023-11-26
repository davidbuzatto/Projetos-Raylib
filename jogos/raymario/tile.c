#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "main.h"

void drawTile( Tile *tile ) {

    SpriteData *data = &tile->data;

    if ( tile->visible ) {
        DrawRectangle( 
            (int) data->x, (int) data->y,
            (int) data->width, (int) data->height, 
            data->baseColor );
    }

}

TileCollision interceptsTyle( Tile *tile, Player *player ) {

    TileCollision r = {
        .tile = NULL,
        .type = COLLISION_NONE
    };

    if ( tile->collideable ) {

        Rectangle rTile = { 
            .x = tile->data.x,
            .y = tile->data.y,
            .width = tile->data.width,
            .height = tile->data.height,
        };

        Rectangle rPlayer = { 
            .x = player->data.x,
            .y = player->data.y,
            .width = player->data.width,
            .height = player->data.height,
        };

        if ( CheckCollisionRecs( rTile, rPlayer ) ) {
            
            SpriteData *pData = &player->data;
            SpriteData *tData = &tile->data;
            r.tile = tile;

            /*if ( pData->x + pData->width >= tData->x && 
                 pData->x + pData->width <= tData->x + tData->width ) {
                r.type = COLLISION_LEFT;
            } else if ( pData->x <= tData->x + tData->width && 
                        pData->x >= tData->x ) {
                r.type = COLLISION_RIGHT;
            }*/

            if ( pData->y + pData->height >= tData->y && 
                 pData->y + pData->height <= tData->y + tData->height ) {
                r.type = COLLISION_TOP;
            } else if ( pData->y <= tData->y + tData->height && 
                        pData->y >= tData->y ) {
                r.type = COLLISION_BOTTOM;
            }

        }

    }

    return r;
}