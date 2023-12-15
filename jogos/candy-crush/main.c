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
#include "include/utils.h"

const bool LOAD_TEST_MAP = true;
const bool REMOVAL_TESTS = true;

const int LINES = 13;
const int COLUMNS = 25;

//const int LINES = 130;
//const int COLUMNS = 200;

typedef struct GameWorld {
    int lines;
    int columns;
    Piece *pieces;
} GameWorld;

int xPress;
int yPress;
int xOffset;
int yOffset;

int xSelected;
int ySelected;
Piece *selectedPiece;

int xExchange;
int yExchange;
Piece *exchangePiece;

GameWorld gw;
Color backgroundColor;
Color lineDetailColor;
Color columnDetailColor;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

Piece* getPiece( GameWorld *gw, int line, int column );

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

    int xMouse;
    int yMouse;
    double relativeAngle;
    Piece *piece;

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {

        xPress = GetMouseX();
        yPress = GetMouseY();

        for ( int i = 0; i < gw->lines; i++ ) {
            for ( int j = 0; j < gw->columns; j++ ) {
                piece = &gw->pieces[i*gw->columns + j];
                if ( coordVsPieceIntercept( piece, xPress, yPress ) ) {
                    selectedPiece = piece;
                    xSelected = selectedPiece->x;
                    ySelected = selectedPiece->y;
                    xOffset = xPress - selectedPiece->x;
                    yOffset = yPress - selectedPiece->y;
                    break;
                }
            }
        }

    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {

        xMouse = GetMouseX();
        yMouse = GetMouseY();

        if ( selectedPiece != NULL ) {

            PieceMovement movingTo = PIECE_MOVEMENT_NONE;
            relativeAngle = toDegrees( atan2( yMouse - yPress, xMouse - xPress ) ) + 180;

            /*char str[10];
            sprintf( str, "%.2f", relativeAngle );
            DrawText( str, xMouse + 20, yMouse, 20, BLACK );
            DrawLine( xPress, yPress, xMouse, yMouse, BLACK );*/

            // which side?
            if ( relativeAngle >= 45 && relativeAngle < 135 ) {
                selectedPiece->x = xPress - xOffset;
                selectedPiece->y = yMouse - yOffset;
                movingTo = PIECE_MOVEMENT_UP;
            } else if ( relativeAngle >= 135 && relativeAngle < 225 ) {
                selectedPiece->x = xMouse - xOffset;
                selectedPiece->y = yPress - yOffset;
                movingTo = PIECE_MOVEMENT_RIGHT;
            } else if ( relativeAngle >= 225 && relativeAngle < 315 ) {
                selectedPiece->x = xPress - xOffset;
                selectedPiece->y = yMouse - yOffset;
                movingTo = PIECE_MOVEMENT_DOWN;
            } else {
                selectedPiece->x = xMouse - xOffset;
                selectedPiece->y = yPress - yOffset;
                movingTo = PIECE_MOVEMENT_LEFT;
            }

            // screen boundaries
            if ( selectedPiece->x + selectedPiece->size > GetScreenWidth() ) {
                selectedPiece->x = GetScreenWidth() - selectedPiece->size;
            } else if ( selectedPiece->x < 0 ) {
                selectedPiece->x = 0;
            } else if ( selectedPiece->y + selectedPiece->size > GetScreenHeight() ) {
                selectedPiece->y = GetScreenHeight() - selectedPiece->size;
            } else if ( selectedPiece->y < 0 ) {
                selectedPiece->y = 0;
            }

            // neighbor boundaries
            if ( selectedPiece->x > xSelected + selectedPiece->size ) {
                selectedPiece->x = xSelected + selectedPiece->size;
            } else if ( selectedPiece->x < xSelected - selectedPiece->size ) {
                selectedPiece->x = xSelected - selectedPiece->size;
            } if ( selectedPiece->y > ySelected + selectedPiece->size ) {
                selectedPiece->y = ySelected + selectedPiece->size;
            } else if ( selectedPiece->y < ySelected - selectedPiece->size ) {
                selectedPiece->y = ySelected - selectedPiece->size;
            }

            // piece exchange logic
            Piece *exchangeWith = NULL;
            switch ( movingTo ) {
                case PIECE_MOVEMENT_LEFT:
                    exchangeWith = getPiece( gw, selectedPiece->line, selectedPiece->column-1 );
                    break;
                case PIECE_MOVEMENT_RIGHT:
                    exchangeWith = getPiece( gw, selectedPiece->line, selectedPiece->column+1 );
                    break;
                case PIECE_MOVEMENT_UP:
                    exchangeWith = getPiece( gw, selectedPiece->line-1, selectedPiece->column );
                    break;
                case PIECE_MOVEMENT_DOWN:
                    exchangeWith = getPiece( gw, selectedPiece->line+1, selectedPiece->column );
                    break;
                default:
                    break;
            }

            if ( exchangeWith != NULL ) {
                if ( exchangePiece == NULL ) {
                    exchangePiece = exchangeWith;
                    xExchange = exchangePiece->x;
                    yExchange = exchangePiece->y;
                } else if ( exchangePiece == exchangeWith ) {
                    switch ( movingTo ) {
                        case PIECE_MOVEMENT_LEFT:
                            exchangePiece->x = xExchange + abs( xSelected - selectedPiece->x );
                            break;
                        case PIECE_MOVEMENT_RIGHT:
                            exchangePiece->x = xExchange - abs( xSelected - selectedPiece->x );
                            break;
                        case PIECE_MOVEMENT_UP:
                            exchangePiece->y = yExchange + abs( ySelected - selectedPiece->y );
                            break;
                        case PIECE_MOVEMENT_DOWN:
                            exchangePiece->y = yExchange - abs( ySelected - selectedPiece->y );
                            break;
                        default:
                            break;
                    }
                } else {
                    exchangePiece->x = xExchange;
                    exchangePiece->y = yExchange;
                    exchangePiece = exchangeWith;
                    xExchange = exchangePiece->x;
                    yExchange = exchangePiece->y;
                }
            }

        }

    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {

        bool removed = false;
        if ( selectedPiece != NULL && exchangePiece != NULL ) {
            int dx = abs( xSelected - selectedPiece->x );
            int dy = abs( ySelected - selectedPiece->y );
            if ( dx >= PIECE_SIZE / 2 || dy >= PIECE_SIZE / 2 ) {
                
                // change positions
                swapLinesAndColumnsFromPieces( selectedPiece, exchangePiece );
                selectedPiece->x = xExchange;
                selectedPiece->y = yExchange;
                exchangePiece->x = xSelected;
                exchangePiece->y = ySelected;

                // verify if it will be removed
                removed = false; // verify()...

                if ( removed ) {
                    // map reposition
                    // chain reaction for more removals...
                    selectedPiece = NULL;
                    exchangePiece = NULL;
                } else {
                    // the pieces will not be removed
                    swapLinesAndColumnsFromPieces( selectedPiece, exchangePiece );
                }

            }
        }

        if ( !removed ) {

            if ( selectedPiece != NULL ) {
                selectedPiece->x = xSelected;
                selectedPiece->y = ySelected;
                selectedPiece = NULL;
            }

            if ( exchangePiece != NULL ) {
                exchangePiece->x = xExchange;
                exchangePiece->y = yExchange;
                exchangePiece = NULL;
            }

        }
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    int externalMargin = 3 * PIECE_SCALE;
    int internalMargin = 1 * PIECE_SCALE;

    DrawRectangleRounded( (Rectangle){
        .x = externalMargin,
        .y = externalMargin,
        .width = GetScreenWidth() - externalMargin * 2,
        .height = GetScreenHeight() - externalMargin * 2
    }, .05, 10, backgroundColor );

    for ( int i = 1; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            DrawLine( 
                externalMargin + internalMargin + j * PIECE_SIZE + 2 * PIECE_SCALE, 
                i * PIECE_SIZE, 
                (j+1) * PIECE_SIZE - externalMargin - internalMargin * 2, 
                i * PIECE_SIZE, 
                lineDetailColor );
        }
    }

    for ( int j = 1; j < gw->columns; j++ ) {
        DrawLine( 
            j * PIECE_SIZE,
            externalMargin + internalMargin, 
            j * PIECE_SIZE, 
            GetScreenHeight() - externalMargin - internalMargin, 
            columnDetailColor );
    }

    if ( REMOVAL_TESTS ) {

        int lStart[] = { 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 6, 6, 6, 6, 10, 10, 10, 10, 7, 5, 10 };
        int cStart[] = { 0, 4, 8, 12, 16, 21, 0, 5, 10, 15, 0, 4, 8, 12, 0, 5, 10, 15, 16, 21, 20 };
        int lEnd[]   = { 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 3 };
        int cEnd[]   = { 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 4, 3, 4 };

        for ( int i = 0; i < 21; i++ ) {
            DrawRectangle( 
                cStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                lStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                cEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                lEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                backgroundColor );
            DrawRectangleLines( 
                cStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                lStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                cEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                lEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                DARKBLUE );
        }

    }

    for ( int i = 0; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            Piece *piece = &gw->pieces[i*gw->columns + j];
            if ( selectedPiece != piece ) {
                drawPiece( piece );
            }
        }
    }

    if ( selectedPiece != NULL ) {
        drawPiece( selectedPiece );
    }

    EndDrawing();

}

Piece* getPiece( GameWorld *gw, int line, int column ) {

    if ( line >= 0 && line < gw->lines && column >= 0 && column < gw->columns ) {
        return &gw->pieces[line*gw->columns + column];
    }

    return NULL;

}

void loadResources( void ) {
    loadPieceResources();
}

void unloadResources( void ) {
    unloadPieceResources();
}

void createGameWorld( int lines, int columns ) {

    if ( LOAD_TEST_MAP ) {

        Piece *pieces = createPiecesFromMap( "resources/maps/testMap.txt", &lines, &columns );
        gw = (GameWorld){
            .lines = lines,
            .columns = columns,
            .pieces = pieces
        };

    } else {
        gw = (GameWorld){
            .lines = lines,
            .columns = columns,
            .pieces = createPieces( lines, columns )
        };
    }

    selectedPiece = NULL;
    exchangePiece = NULL;

    backgroundColor = (Color){ .r = 58, .g = 91, .b = 147, .a = 200 };
    lineDetailColor = (Color){ .r = 217, .g = 225, .b = 240, .a = 200 };
    columnDetailColor = (Color){ .r = 28, .g = 44, .b = 70, .a = 200 };

}

void destroyGameWorld( void ) {
    free( gw.pieces );
}