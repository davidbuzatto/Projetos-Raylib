#pragma once

#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

extern const int PIECE_SIZE;

typedef enum PieceMovement {
    PIECE_MOVEMENT_LEFT,
    PIECE_MOVEMENT_RIGHT,
    PIECE_MOVEMENT_UP,
    PIECE_MOVEMENT_DOWN,
    PIECE_MOVEMENT_NONE
} PieceMovement;

typedef enum PieceType {
    PLAIN_RED,
    PLAIN_ORANGE,
    PLAIN_YELLOW,
    PLAIN_GREEN,
    PLAIN_BLUE,
    PLAIN_PURPLE
} PieceType;

typedef struct Piece {
    bool initialized;
    int line;
    int column;
    int x;
    int y;
    int size;
    PieceType type;
    Texture2D *textureMap;
} Piece;

void drawPiece( Piece *piece );
Piece *createPieces( int lines, int columns );
bool coordVsPieceIntercept( Piece *piece, int x, int y );
void loadPieceResources( void );
void unloadPieceResources( void );