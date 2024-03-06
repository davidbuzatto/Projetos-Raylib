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

GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->dummy = 0;

    return gw;

}

void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

void inputAndUpdateGameWorld( GameWorld *gw ) {

}

const int LARGURA = 100;
const int ALTURA = 100;

float funcao1( float x ) {
    return ALTURA - x;
}

float funcao2( float x ) {
    return ALTURA - x + LARGURA;
}

float funcao3( float x ) {
    return ALTURA - x + 2 * LARGURA;
}

void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    Vector2 pMouse = GetMousePosition();
    int xIniTile1 = 0;
    int xFimTile1 = LARGURA;
    int xIniTile2 = LARGURA;
    int xFimTile2 = 2 * LARGURA;
    int xIniTile3 = 2 * LARGURA;
    int xFimTile3 = 3 * LARGURA;

    for ( int x = xIniTile1; x <= xFimTile1; x++ ) {
        float y = funcao1( x );
        DrawPixel( x, y, BLACK );
    }

    for ( int x = xIniTile2; x <= xFimTile2; x++ ) {
        float y = funcao2( x );
        DrawPixel( x, y, BLUE );
    }

    for ( int x = xIniTile3; x <= xFimTile3; x++ ) {
        float y = funcao3( x );
        DrawPixel( x, y, DARKGREEN );
    }

    if ( pMouse.x >= xIniTile1 && pMouse.x <= xFimTile1 &&
         funcao1(pMouse.x) < pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile1, funcao1(xFimTile1), LARGURA, ALTURA, Fade( BLACK, 0.5 ) );
    }

    if ( pMouse.x >= xIniTile1 && pMouse.x <= xFimTile1 &&
         funcao1(pMouse.x) > pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile1, funcao1(xFimTile1), LARGURA, ALTURA, Fade( GREEN, 0.5 ) );
    }

    if ( pMouse.x >= xIniTile2 && pMouse.x <= xFimTile2 &&
         funcao2(pMouse.x) < pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile2, funcao2(xFimTile2), LARGURA, ALTURA, Fade( BLACK, 0.5 ) );
    }

    if ( pMouse.x >= xIniTile2 && pMouse.x <= xFimTile2 &&
         funcao2(pMouse.x) > pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile2, funcao2(xFimTile2), LARGURA, ALTURA, Fade( GREEN, 0.5 ) );
    }

    if ( pMouse.x >= xIniTile3 && pMouse.x <= xFimTile3 &&
         funcao3(pMouse.x) < pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile3, funcao3(xFimTile3), LARGURA, ALTURA, Fade( BLACK, 0.5 ) );
    }

    if ( pMouse.x >= xIniTile3 && pMouse.x <= xFimTile3 &&
         funcao3(pMouse.x) > pMouse.y && pMouse.y <= ALTURA ) {
        DrawRectangle( xIniTile3, funcao3(xFimTile3), LARGURA, ALTURA, Fade( GREEN, 0.5 ) );
    }


    EndDrawing();

}