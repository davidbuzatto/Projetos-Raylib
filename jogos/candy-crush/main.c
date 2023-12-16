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
Vector2 removalPositions[5];

const int LINES = 16;
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

bool verify3Horizontal( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify3Vertical( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify4Horizontal( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify4Vertical( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify5L( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify5T( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );
bool verify5Star( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece );

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

                swapDataAndPointers( &selectedPiece, &exchangePiece );

                /*if ( verify5L( gw, selectedPiece, exchangePiece ) ||
                     verify5L( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 5 L!\n" );
                    removed = true;
                } else if ( verify5T( gw, selectedPiece, exchangePiece ) ||
                            verify5T( gw, exchangePiece, selectedPiece )) {
                    printf( "removing 5 T!\n" );
                    removed = true;
                } else if ( verify5Star( gw, selectedPiece, exchangePiece ) ||
                            verify5Star( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 5 star!\n" );
                    removed = true;
                } else if ( verify4Horizontal( gw, selectedPiece, exchangePiece ) ||
                            verify4Horizontal( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 4 horizontal!\n" );
                    removed = true;
                } else if ( verify4Vertical( gw, selectedPiece, exchangePiece ) || 
                            verify4Vertical( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 4 vertical!\n" );
                    removed = true;
                } else if ( verify3Horizontal( gw, selectedPiece, exchangePiece ) || 
                            verify3Horizontal( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 3 horizontal!\n" );
                    removed = true;
                } else if ( verify3Vertical( gw, selectedPiece, exchangePiece ) || 
                            verify3Vertical( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing 3 vertical!\n" );
                    removed = true;
                }*/

                removalPositions[0] = (Vector2){ .x = -1, .y = -1 };
                removalPositions[1] = (Vector2){ .x = -1, .y = -1 };
                removalPositions[2] = (Vector2){ .x = -1, .y = -1 };
                removalPositions[3] = (Vector2){ .x = -1, .y = -1 };
                removalPositions[4] = (Vector2){ .x = -1, .y = -1 };

                if (          verify5L( gw, selectedPiece, exchangePiece ) ||
                              verify5L( gw, exchangePiece, selectedPiece ) ||
                              verify5T( gw, selectedPiece, exchangePiece ) ||
                              verify5T( gw, exchangePiece, selectedPiece ) ||
                           verify5Star( gw, selectedPiece, exchangePiece ) ||
                           verify5Star( gw, exchangePiece, selectedPiece ) ||
                     verify4Horizontal( gw, selectedPiece, exchangePiece ) ||
                     verify4Horizontal( gw, exchangePiece, selectedPiece ) ||
                       verify4Vertical( gw, selectedPiece, exchangePiece ) || 
                       verify4Vertical( gw, exchangePiece, selectedPiece ) ||
                     verify3Horizontal( gw, selectedPiece, exchangePiece ) || 
                     verify3Horizontal( gw, exchangePiece, selectedPiece ) ||
                       verify3Vertical( gw, selectedPiece, exchangePiece ) || 
                       verify3Vertical( gw, exchangePiece, selectedPiece ) ) {
                    printf( "removing...\n" );
                    removed = true;
                }

                if ( !removed ) {
                    swapDataAndPointers( &selectedPiece, &exchangePiece );
                }

            }
        }

        if ( removed ) {
            selectedPiece = NULL;
            exchangePiece = NULL;
        } else {

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
    }, .05 * PIECE_SCALE, 10, backgroundColor );

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

        int lStart[] = { 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 13, 13, 13, 13, 9, 6, 13 };
        int cStart[] = { 0, 4, 8, 12, 16, 21, 0, 5, 10, 15, 0, 4, 8, 12, 0, 5, 10, 15, 16, 21, 20 };
        int lEnd[]   = { 3, 3, 3, 3, 3, 4, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 3, 4, 3 };
        int cEnd[]   = { 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 3, 4, 4, 4, 4, 4, 3, 4 };

        for ( int i = 0; i < 21; i++ ) {
            DrawRectangle( 
                cStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                lStart[i] * PIECE_SIZE + 5 * PIECE_SCALE, 
                cEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                lEnd[i] * PIECE_SIZE - 10 * PIECE_SCALE, 
                lineDetailColor );
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

    for ( int i = 0; i < 5; i++ ) {
        Vector2 p = removalPositions[i];
        if ( p.x != -1 ) {
            DrawCircle( p.x * PIECE_SIZE + PIECE_SIZE / 2, p.y * PIECE_SIZE + PIECE_SIZE / 2, PIECE_SIZE / 4, BLACK );
        }
    }

    EndDrawing();

}

Piece* getPiece( GameWorld *gw, int line, int column ) {

    if ( line >= 0 && line < gw->lines && column >= 0 && column < gw->columns ) {
        return &gw->pieces[line*gw->columns + column];
    }

    return NULL;

}

bool verify3Horizontal( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line, exchangePiece->column-2 );
    Piece *p2 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    Piece *p3 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );
    Piece *p4 = getPiece( gw, exchangePiece->line, exchangePiece->column+2 );

    if ( p1 != NULL && p2 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        return true;
    }

    if ( p2 != NULL && p3 != NULL && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[2] = (Vector2){ .x = p3->column, .y = p3->line };
        return true;
    }

    if ( p3 != NULL && p4 != NULL && 
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[2] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    return false;

}

bool verify3Vertical( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line-2, exchangePiece->column );
    Piece *p2 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    Piece *p3 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );
    Piece *p4 = getPiece( gw, exchangePiece->line+2, exchangePiece->column );

    if ( p1 != NULL && p2 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        return true;
    }

    if ( p2 != NULL && p3 != NULL && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[2] = (Vector2){ .x = p3->column, .y = p3->line };
        return true;
    }

    if ( p3 != NULL && p4 != NULL && 
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[2] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    return false;

}

bool verify4Horizontal( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line, exchangePiece->column-3 );
    Piece *p2 = getPiece( gw, exchangePiece->line, exchangePiece->column-2 );
    Piece *p3 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    Piece *p4 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );
    Piece *p5 = getPiece( gw, exchangePiece->line, exchangePiece->column+2 );
    Piece *p6 = getPiece( gw, exchangePiece->line, exchangePiece->column+3 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL &&
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        return true;
    }

    if ( p2 != NULL && p3 != NULL && p4 != NULL &&
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[2] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[3] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    if ( p3 != NULL && p4 != NULL && p5 != NULL &&
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type && 
            selectedPiece->type == p5->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[2] = (Vector2){ .x = p4->column, .y = p4->line };
        removalPositions[3] = (Vector2){ .x = p5->column, .y = p5->line };
        return true;
    }

    if ( p4 != NULL && p5 != NULL && p6 != NULL &&
            selectedPiece->type == p4->type && 
            selectedPiece->type == p5->type && 
            selectedPiece->type == p6->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p4->column, .y = p4->line };
        removalPositions[2] = (Vector2){ .x = p5->column, .y = p5->line };
        removalPositions[3] = (Vector2){ .x = p6->column, .y = p6->line };
        return true;
    }

    return false;

}

bool verify4Vertical( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line-3, exchangePiece->column );
    Piece *p2 = getPiece( gw, exchangePiece->line-2, exchangePiece->column );
    Piece *p3 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    Piece *p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );
    Piece *p5 = getPiece( gw, exchangePiece->line+2, exchangePiece->column );
    Piece *p6 = getPiece( gw, exchangePiece->line+3, exchangePiece->column );

    if ( p1 != NULL && p2 != NULL && p3 != NULL &&
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        return true;
    }

    if ( p2 != NULL && p3 != NULL && p4 != NULL &&
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[2] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[3] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    if ( p3 != NULL && p4 != NULL && p5 != NULL &&
            selectedPiece->type == p3->type && 
            selectedPiece->type == p4->type && 
            selectedPiece->type == p5->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[2] = (Vector2){ .x = p4->column, .y = p4->line };
        removalPositions[3] = (Vector2){ .x = p5->column, .y = p5->line };
        return true;
    }

    if ( p4 != NULL && p5 != NULL && p6 != NULL &&
            selectedPiece->type == p4->type && 
            selectedPiece->type == p5->type && 
            selectedPiece->type == p6->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p4->column, .y = p4->line };
        removalPositions[2] = (Vector2){ .x = p5->column, .y = p5->line };
        removalPositions[3] = (Vector2){ .x = p6->column, .y = p6->line };
        return true;
    }

    return false;

}

bool verify5L( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1Left = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    Piece *p2Left = getPiece( gw, exchangePiece->line, exchangePiece->column-2 );
    Piece *p1Right = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );
    Piece *p2Right = getPiece( gw, exchangePiece->line, exchangePiece->column+2 );
    Piece *p1Up = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    Piece *p2Up = getPiece( gw, exchangePiece->line-2, exchangePiece->column );
    Piece *p1Down = getPiece( gw, exchangePiece->line+1, exchangePiece->column );
    Piece *p2Down = getPiece( gw, exchangePiece->line+2, exchangePiece->column );

    if ( p1Up != NULL && p2Up != NULL && p1Right != NULL && p2Right != NULL && 
            selectedPiece->type == p1Up->type && 
            selectedPiece->type == p2Up->type && 
            selectedPiece->type == p1Right->type &&
            selectedPiece->type == p2Right->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1Up->column, .y = p1Up->line };
        removalPositions[2] = (Vector2){ .x = p2Up->column, .y = p2Up->line };
        removalPositions[3] = (Vector2){ .x = p1Right->column, .y = p1Right->line };
        removalPositions[4] = (Vector2){ .x = p2Right->column, .y = p2Right->line };
        return true;
    }

    if ( p1Right != NULL && p2Right != NULL && p1Down != NULL && p2Down != NULL && 
            selectedPiece->type == p1Right->type && 
            selectedPiece->type == p2Right->type && 
            selectedPiece->type == p1Down->type &&
            selectedPiece->type == p2Down->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1Right->column, .y = p1Right->line };
        removalPositions[2] = (Vector2){ .x = p2Right->column, .y = p2Right->line };
        removalPositions[3] = (Vector2){ .x = p1Down->column, .y = p1Down->line };
        removalPositions[4] = (Vector2){ .x = p2Down->column, .y = p2Down->line };
        return true;
    }

    if ( p1Down != NULL && p2Down != NULL && p1Left != NULL && p2Left != NULL && 
            selectedPiece->type == p1Down->type && 
            selectedPiece->type == p2Down->type && 
            selectedPiece->type == p1Left->type &&
            selectedPiece->type == p2Left->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1Down->column, .y = p1Down->line };
        removalPositions[2] = (Vector2){ .x = p2Down->column, .y = p2Down->line };
        removalPositions[3] = (Vector2){ .x = p1Left->column, .y = p1Left->line };
        removalPositions[4] = (Vector2){ .x = p2Left->column, .y = p2Left->line };
        return true;
    }

    if ( p1Left != NULL && p2Left != NULL && p1Up != NULL && p2Up != NULL && 
            selectedPiece->type == p1Left->type && 
            selectedPiece->type == p2Left->type && 
            selectedPiece->type == p1Up->type &&
            selectedPiece->type == p2Up->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1Left->column, .y = p1Left->line };
        removalPositions[2] = (Vector2){ .x = p2Left->column, .y = p2Left->line };
        removalPositions[3] = (Vector2){ .x = p1Up->column, .y = p1Up->line };
        removalPositions[4] = (Vector2){ .x = p2Up->column, .y = p2Up->line };
        return true;
    }

    return false;

}

bool verify5T( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    Piece *p2 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    Piece *p3 = getPiece( gw, exchangePiece->line, exchangePiece->column-2 );
    Piece *p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    p2 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );
    p3 = getPiece( gw, exchangePiece->line, exchangePiece->column+2 );
    p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    p2 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    p3 = getPiece( gw, exchangePiece->line-2, exchangePiece->column );
    p4 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    p2 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );
    p3 = getPiece( gw, exchangePiece->line+2, exchangePiece->column );
    p4 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }
    
    return false;

}

bool verify5Star( GameWorld *gw, Piece *selectedPiece, Piece *exchangePiece ) {

    Piece *p1 = getPiece( gw, exchangePiece->line-1, exchangePiece->column-1 );
    Piece *p2 = getPiece( gw, exchangePiece->line, exchangePiece->column-1 );
    Piece *p3 = getPiece( gw, exchangePiece->line, exchangePiece->column-2 );
    Piece *p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column-1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line-1, exchangePiece->column+1 );
    p2 = getPiece( gw, exchangePiece->line, exchangePiece->column+1 );
    p3 = getPiece( gw, exchangePiece->line, exchangePiece->column+2 );
    p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column+1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line-1, exchangePiece->column-1 );
    p2 = getPiece( gw, exchangePiece->line-1, exchangePiece->column );
    p3 = getPiece( gw, exchangePiece->line-2, exchangePiece->column );
    p4 = getPiece( gw, exchangePiece->line-1, exchangePiece->column+1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }

    p1 = getPiece( gw, exchangePiece->line+1, exchangePiece->column-1 );
    p2 = getPiece( gw, exchangePiece->line+1, exchangePiece->column );
    p3 = getPiece( gw, exchangePiece->line+2, exchangePiece->column );
    p4 = getPiece( gw, exchangePiece->line+1, exchangePiece->column+1 );

    if ( p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
            selectedPiece->type == p1->type && 
            selectedPiece->type == p2->type && 
            selectedPiece->type == p3->type &&
            selectedPiece->type == p4->type ) {
        removalPositions[0] = (Vector2){ .x = exchangePiece->column, .y = exchangePiece->line };
        removalPositions[1] = (Vector2){ .x = p1->column, .y = p1->line };
        removalPositions[2] = (Vector2){ .x = p2->column, .y = p2->line };
        removalPositions[3] = (Vector2){ .x = p3->column, .y = p3->line };
        removalPositions[4] = (Vector2){ .x = p4->column, .y = p4->line };
        return true;
    }
    
    return false;

}

void loadResources( void ) {
    loadPieceResources();
}

void unloadResources( void ) {
    unloadPieceResources();
}

void createGameWorld( int lines, int columns ) {

    if ( LOAD_TEST_MAP ) {

        Piece *pieces = createPiecesFromMap( "resources/maps/testMapRandom.txt", &lines, &columns );
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

    removalPositions[0] = (Vector2){ .x = -1, .y = -1 };
    removalPositions[1] = (Vector2){ .x = -1, .y = -1 };
    removalPositions[2] = (Vector2){ .x = -1, .y = -1 };
    removalPositions[3] = (Vector2){ .x = -1, .y = -1 };
    removalPositions[4] = (Vector2){ .x = -1, .y = -1 };

}

void destroyGameWorld( void ) {
    free( gw.pieces );
}