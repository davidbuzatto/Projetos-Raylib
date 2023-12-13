/*******************************************************************************
 * Candy Crush game using Raylib https://www.raylib.com/
 *  
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "include/piece.h"

typedef struct {
    int lines;
    int columns;
    Piece *pieces;
} GameWorld;

GameWorld gw;

const int LINES = 9;
const int COLUMNS = 5;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void loadResources( void );
void unloadResources( void );

void createGameWorld( int lines, int columns );
void destroyGameWorld( void );

int main( void ) {

    const int screenWidth = COLUMNS * PIECE_SIZE;
    const int screenHeight = LINES * PIECE_SIZE;

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Candy Crush - Prof. Dr. David Buzatto" );
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld( LINES, COLUMNS );

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    destroyGameWorld();
    unloadResources();

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {



}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            Piece *piece = &gw->pieces[i*gw->columns + j];
            drawPiece( piece );
        }
    }

    EndDrawing();

}

void loadResources( void ) {
    loadPieceResources();
}

void unloadResources( void ) {
    unloadPieceResources();
}

void createGameWorld( int lines, int columns ) {

    gw = (GameWorld){
        .lines = lines,
        .columns = columns,
        .pieces = createPieces( lines, columns )
    };

}

void destroyGameWorld( void ) {
    free( gw.pieces );
}