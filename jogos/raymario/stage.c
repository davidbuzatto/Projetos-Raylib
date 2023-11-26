#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "main.h"

void inputAndUpdateStage( Stage *stage ) {


}

void drawStage( Stage *stage ) {

    DrawRectangle( 0, 0, STAGE_WIDTH * TILE_WIDTH, STAGE_HEIGHT * TILE_WIDTH, stage->data.baseColor );

    for ( int i = 0; i < STAGE_HEIGHT; i++ ) {
        for ( int j = 0; j < STAGE_WIDTH; j++ ) {
            drawTile( &stage->terrain[i][j] );
        }
    }

}

void parseTerrain( Stage *stage, const char* terrainData ) {

    const char *current = terrainData;
    int i = 0;
    int j = 0;
    Tile *t;

    while ( *current != '\0' ) {

        t = &(stage->terrain[i][j]);
        t->data.width = TILE_WIDTH;
        t->data.height = TILE_WIDTH;

        switch ( *current ) {
            case '#': 
                t->data.x = j * TILE_WIDTH;
                t->data.y = i * TILE_WIDTH;
                t->data.baseColor = BLACK;
                t->collideable = true;
                t->visible = true;
                j++;
                break;
            case '%': 
                t->data.x = j * TILE_WIDTH;
                t->data.y = i * TILE_WIDTH;
                t->data.baseColor = PURPLE;
                t->collideable = true;
                t->visible = true;
                j++;
                break;
            case ' ': 
                t->data.x = j * TILE_WIDTH;
                t->data.y = i * TILE_WIDTH;
                t->data.baseColor = GREEN;
                t->collideable = false;
                t->visible = true;
                j++;
                break;
            case '\n': 
                i++; 
                j = 0; 
                break;
        }

        current++;

    }

}

TileCollision interceptsStage( Stage *stage, Player *player ) {

    TileCollision r = {
        .tile = NULL,
        .type = COLLISION_TYPE_NONE
    };

    for ( int i = 0; i < STAGE_HEIGHT; i++ ) {
        for ( int j = 0; j < STAGE_WIDTH; j++ ) {
            r = interceptsTyle( &stage->terrain[i][j], player );
            if ( r.type != COLLISION_TYPE_NONE ) {
                return r;
            }
        }
    }

    return r;

}