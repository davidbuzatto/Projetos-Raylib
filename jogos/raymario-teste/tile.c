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
        .tile = tile,
        .type = COLLISION_TYPE_NONE
    };

    if ( tile->collideable ) {

        Rectangle rPlayer = { 
            .x = player->data.x,
            .y = player->data.y,
            .width = player->data.width,
            .height = player->data.height,
        };

        Rectangle rTile = { 
            .x = tile->data.x,
            .y = tile->data.y,
            .width = tile->data.width,
            .height = tile->data.height,
        };
        
        r.type = collide( &rPlayer, &rTile );

    }

    return r;
}