/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
//#include "raymath.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//#undef RAYGUI_IMPLEMENTATION

#include "GameWorld.h"
#include "ResourceManager.h"
#include "Tile.h"

GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->tiles[0] = (Tile){
        .pos = { 50, 50 },
        .dim = { 100, 100 },
        .type = TILE_TYPE_SLOPE_UP,
        .color = BLUE
    };

    gw->tiles[1] = (Tile){
        .pos = { 200, 50 },
        .dim = { 100, 150 },
        .type = TILE_TYPE_SLOPE_UP,
        .color = DARKGREEN
    };

    gw->tiles[2] = (Tile){
        .pos = { 350, 50 },
        .dim = { 200, 100 },
        .type = TILE_TYPE_SLOPE_UP,
        .color = PURPLE
    };

    gw->tiles[3] = (Tile){
        .pos = { 50, 250 },
        .dim = { 100, 100 },
        .type = TILE_TYPE_SLOPE_DOWN,
        .color = BLUE
    };

    gw->tiles[4] = (Tile){
        .pos = { 200, 250 },
        .dim = { 100, 150 },
        .type = TILE_TYPE_SLOPE_DOWN,
        .color = DARKGREEN
    };

    gw->tiles[5] = (Tile){
        .pos = { 350, 250 },
        .dim = { 200, 100 },
        .type = TILE_TYPE_SLOPE_DOWN,
        .color = PURPLE
    };

    gw->tileQuantity = 6;

    return gw;

}

void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

void inputAndUpdateGameWorld( GameWorld *gw ) {
    Vector2 mousePos = GetMousePosition();
    for ( int i = 0; i < gw->tileQuantity; i++ ) {
        gw->tiles[i].collisionType = checkCollisionTileVector2( &gw->tiles[i], mousePos );
    }
}

void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < gw->tileQuantity; i++ ) {
        drawTile( &gw->tiles[i] );
    }

    EndDrawing();

}