#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "base.h"
#include "stage.h"
#include "tile.h"

void updateStage( Stage *stage ) {

    // slide right or left
    // paralax

}

void drawStage( Stage *stage ) {

    SpriteData *data = &stage->data;

    for ( int i = 0; i < 15; i++ ) {
        for ( int j = 0; j < 20; j++ ) {
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
                t->collideable = false;
                t->visible = false;
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