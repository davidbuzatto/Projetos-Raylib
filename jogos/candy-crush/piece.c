#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "include/piece.h"

const int PIECE_SIZE = 100;

static Texture2D pieceTextureMap;

void drawPiece( Piece *piece ) {

    if ( piece->initialized ) {

        int marginX = 0;
        int marginY = 282;

        switch ( piece->type ) {
            case PLAIN_RED:    marginX = 16; break;
            case PLAIN_ORANGE: marginX = 150; break;
            case PLAIN_YELLOW: marginX = 284; break;
            case PLAIN_GREEN:  marginX = 418; break;
            case PLAIN_BLUE:   marginX = 550; break;
            case PLAIN_PURPLE: marginX = 682; break;
        }

        Rectangle source = {
            .x = marginX,
            .y = marginY,
            .width = piece->size,
            .height = piece->size
        };

        Rectangle dest = {
            .x = piece->x,
            .y = piece->y,
            .width = piece->size,
            .height = piece->size
        };

        Vector2 origin = {
            .x = 0,
            .y = 0
        };

        DrawTexturePro( *(piece->textureMap), source, dest, origin , 0, WHITE );

    }

}

Piece *createPieces( int lines, int columns ) {

    size_t memSize = lines * columns * sizeof( Piece );
    Piece *pieces = (Piece*) malloc( memSize );
    memset( pieces, 0, memSize );

    for ( int i = 0; i < lines; i++ ) {
        for ( int j = 0; j < columns; j++ ) {
            pieces[i*columns + j] = (Piece){
                .initialized = true,
                .line = i,
                .column = j,
                .x = j * PIECE_SIZE,
                .y = i * PIECE_SIZE,
                .size = PIECE_SIZE,
                .type = GetRandomValue( 0, 5 ),
                .textureMap = &pieceTextureMap
            };
        }
        //break;
    }

    return pieces;

}

bool coordVsPieceIntercept( Piece *piece, int x, int y ) {

    if ( piece->initialized ) {
        return x >= piece->x && x <= piece->x + piece->size &&
            y >= piece->y && y <= piece->y + piece->size;
    }

    return false;

}

void loadPieceResources( void ) {
    pieceTextureMap = LoadTexture( "resources/candy_crush_sprites.png" );
}

void unloadPieceResources( void ) {
    UnloadTexture( pieceTextureMap );
}