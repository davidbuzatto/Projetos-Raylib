#pragma once

#include "raylib.h"
#include "raymath.h"

extern const int PIECE_SIZE;

typedef enum PieceType {
    PLAIN_RED,
    PLAIN_ORANGE,
    PLAIN_YELLOW,
    PLAIN_GREEN,
    PLAIN_BLUE,
    PLAIN_PURPLE
} PieceType;

typedef struct Piece {
    int line;
    int column;
    int size;
    PieceType type;
    Texture2D *textureMap;
} Piece;

void drawPiece( Piece *piece );
Piece *createPieces( int lines, int columns );
void loadPieceResources( void );
void unloadPieceResources( void );