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
#include "utils.h"

#define ARRAY_SIZE 10

GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    *gw = (GameWorld){ 0 };

    gw->arraySize = ARRAY_SIZE;
    gw->array = (int*) malloc( sizeof( int ) * gw->arraySize );
    gw->finished = false;
    gw->timeToWait = .5;

    for ( size_t i = 0; i < gw->arraySize; i++ ) {
        gw->array[i] = i+1;
    }

    for ( size_t i = 0; i < gw->arraySize; i++ ) {
        swap( gw->array, i, GetRandomValue( 0, gw->arraySize - 1 ) );
    }

    return gw;

}

void destroyGameWorld( GameWorld *gw ) {
    free( gw->array );
    free( gw );
}

void inputAndUpdateGameWorld( GameWorld *gw ) {

    if ( !gw->finished ) {
        gw->timeAcum += GetFrameTime();
        if ( gw->timeAcum >= gw->timeToWait ) {
            gw->timeAcum = 0;
            next( gw );
        }
    }

}

void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    int spacing = 2;
    int margin = 20;
    int spaces = spacing * ( gw->arraySize - 1 );
    int valueWidth = ( GetScreenWidth() - margin * 2 - spaces ) / gw->arraySize;
    int valueHeight = ( GetScreenHeight() - margin * 2 ) / gw->arraySize;
    int iniY = GetScreenHeight() - margin;
    
    for ( size_t i = 0; i < gw->arraySize; i++ ) {
        int height = gw->array[i] * valueHeight;
        if ( gw->min == i ) {
            DrawRectangle( margin + ( valueWidth + spacing ) * i, iniY - height, valueWidth, height, RED );
        } else if( gw->i == i ) {
            DrawRectangle( margin + ( valueWidth + spacing ) * i, iniY - height, valueWidth, height, PURPLE );
        } else if( gw->j == i ) {
            DrawRectangle( margin + ( valueWidth + spacing ) * i, iniY - height, valueWidth, height, GREEN );
        } else {
            DrawRectangle( margin + ( valueWidth + spacing ) * i, iniY - height, valueWidth, height, BLUE );
        }
    }

    EndDrawing();

}

void next( GameWorld *gw ) {

    int* array = gw->array;
    size_t arraySize = gw->arraySize;

    if ( gw->i < arraySize ) {
        if ( !gw->minInit ) {
            gw->min = gw->i;
            gw->minInit = true;
            gw->j = gw->i + 1;
        }
        if ( gw->j < arraySize ) {
            if ( array[gw->min] > array[gw->j] ) {
                gw->min = gw->j;
            }
            gw->j++;
        }
        if ( gw->j == arraySize ) {
            swap( array, gw->min, gw->i );
            gw->i++;
            gw->minInit = false;
        }
    } else {
        gw->finished = true;
    }

}