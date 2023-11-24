/*******************************************************************************
 * Tetris game using Raylib (https://www.raylib.com/)
 * 
 * 
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

/**
 * TODO list:
 *     verify completed lines (sum to score);
 *     increase speed (time to time or based on score value?);
 *     collisions for left and right;
 *     pause screen;
 *         unpause;
 *         reset;
 *     start screen;
 *         play;
 *     modularize;
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "tetrominoe_h.h"
#include "tetrominoe_il.h"
#include "tetrominoe_l.h"
#include "tetrominoe_sq.h"
#include "tetrominoe_s.h"
#include "tetrominoe_z.h"
#include "tetrominoe_t.h"

// macros

// enums, structs, unions and custom types
typedef enum {
    H  = 1, // using values to make possible use 0 in board and get rid of compiler warnings
    IL = 2,
    L  = 3,
    SQ = 4,
    S  = 5,
    Z  = 6,
    T  = 7
} PieceType;

typedef enum {
    PLAYING,
    STOPPED,
    LOSE
} GameState;

// Tetrominoe representation
typedef struct {
    int line;
    int column;
    int width;
    int height;
    bool data[4][4][4];
    int currentFrame;
    int dimensions[4][4];
    PieceType type;
    Color color;
    int movementCount;
    int maxCount;
    bool stopped;
} Piece;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int points;
} ScoreBox;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int lines;
} LinesBox;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    Piece *nextPiece;
} NextPieceBox;

typedef struct {
    PieceType board[20][10]; // consolidated pieces positions
    Piece *currentPiece;
    ScoreBox *scoreBox;
    LinesBox *linesBox;
    NextPieceBox *nextPieceBox;
    GameState state;
} GameWorld;

// global variables
const Color BACKGROUND_COLOR = { .r = 230, .g = 230, .b = 230, .a = 255 };

// function prototypes
void input( GameWorld *gw );
void update( GameWorld *gw );
void draw( GameWorld *gw );

void createPiece( PieceType type, int line, int column, GameWorld *gw );
void createRandomPiece( GameWorld *gw );
void updatePiece( Piece *piece );
void drawPiece( Piece *piece );
void turnPiece( Piece *piece );
void checkPieceBoundaries( Piece *piece );
void stopWhenReachedBottom( Piece *piece );
void stopWhenCollide( Piece *piece, GameWorld *gw );
void copyPieceToBoard( Piece *piece, GameWorld *gw );
void processBoardToScore( GameWorld *gw );

void drawScoreBox( ScoreBox *score );
void drawLinesBox( LinesBox *score );
void drawNextPieceBox( NextPieceBox *box );

Color getColorByPieceType( PieceType type );

int main( void ) {

    // initialization
    const int screenWidth = 600;
    const int screenHeight = 800;

    printf( "%d\n", tetrominoeH[0][0][0] );

    ScoreBox scoreBox = {
        .x = 420,
        .y = 20,
        .width = 160,
        .height = 60,
        .points = 0
    };

    LinesBox linesBox = {
        .x = 420,
        .y = 120,
        .width = 160,
        .height = 60,
        .lines = 0
    };

    NextPieceBox nextPieceBox = {
        .x = 420,
        .y = 220,
        .width = 160,
        .height = 90,
        .nextPiece = NULL
    };

    GameWorld gw = { 
        .currentPiece = NULL,
        .scoreBox = &scoreBox,
        .linesBox = &linesBox,
        .nextPieceBox = &nextPieceBox,
        .state = PLAYING
    };
    memset( gw.board, 0, 200 * sizeof( int ) );

    // test
    /*for ( int i = 17; i < 20; i++ ) {
        gw.board[i][0] = 0;
        gw.board[i][1] = 0;
        gw.board[i][2] = 0;
        gw.board[i][7] = 0;
        gw.board[i][8] = 0;
        gw.board[i][9] = 0;
    }*/

    //createPiece( H, 0, 3, &gw );
    //createPiece( H, 0, 3, &gw );
    //createPiece( IL, 0, 3, &gw );
    //createPiece( L, 0, 3, &gw );
    //createPiece( SQ, 0, 4, &gw );
    //createPiece( S, 0, 3, &gw );
    //createPiece( Z, 0, 3, &gw );
    //createPiece( T, 0, 3, &gw );

    SetRandomSeed( time( NULL ) );
    createRandomPiece( &gw );
    createRandomPiece( &gw );

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Tetris - Prof. Dr. David Buzatto");
    //InitAudioDevice();
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        input( &gw );
        update( &gw );
        draw( &gw );
    }

    //CloseAudioDevice();
    CloseWindow();
    return 0;

}

void input( GameWorld *gw ) {

    Piece *piece = gw->currentPiece;

    if ( gw->state == PLAYING && piece != NULL && !piece->stopped ) {

        if ( IsKeyPressed( KEY_RIGHT ) || IsKeyPressedRepeat( KEY_RIGHT ) ) {
            piece->column++;
        }

        if ( IsKeyPressed( KEY_LEFT ) || IsKeyPressedRepeat( KEY_LEFT ) ) {
            piece->column--;
        }

        if ( IsKeyPressed( KEY_DOWN ) || IsKeyPressedRepeat( KEY_DOWN ) ) {
            piece->line++;
        }

        if ( IsKeyPressed( KEY_UP ) ) {
            turnPiece( piece );
        }

        if ( IsKeyPressed( KEY_SPACE ) ) {
            // TODO
            printf( "go bottom\n" );
        }

    }

}

void update( GameWorld *gw ) {

    Piece *piece = gw->currentPiece;

    if ( gw->state == PLAYING && piece != NULL ) {

        updatePiece( gw->currentPiece );
        checkPieceBoundaries( piece );
        stopWhenReachedBottom( piece );
        stopWhenCollide( piece, gw );

        // check if the player lost
        for ( int i = 0; i < 10; i++ ) {
            if ( gw->board[0][i] != 0 ) {
                gw->state = LOSE;
                break;
            }
        }

        if ( piece->stopped ) {

            copyPieceToBoard( piece, gw );
            processBoardToScore( gw );

            if ( gw->state == PLAYING ) {
                createRandomPiece( gw );
            }

        }

    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BACKGROUND_COLOR );

    DrawRectangle( 0, 0, 400, 800, BLACK );

    for ( int i = 0; i < 20; i++ ) {
        for ( int j = 0; j < 10; j++ ) {
            if ( gw->board[i][j] != 0 ) {
                DrawRectangle( 
                    j * 40, 
                    i * 40, 
                    40, 40, 
                    gw->state == PLAYING ? getColorByPieceType( gw->board[i][j] ) : DARKGRAY );
            }
        }
    }

    if ( gw->currentPiece != NULL ) {
        drawPiece( gw->currentPiece );
    }

    for ( int i = 0; i <= 20; i++ ) {
        DrawRectangle( 0, i * 40, 400, 2, DARKGRAY );
    }

    for ( int i = 0; i <= 10; i++ ) {
        DrawRectangle( i * 40, 0, 2, 800, DARKGRAY );
    }

    drawScoreBox( gw->scoreBox );
    drawLinesBox( gw->linesBox );
    drawNextPieceBox( gw->nextPieceBox );

    EndDrawing();

}

void createPiece( PieceType type, int line, int column, GameWorld *gw ) {

    if ( gw->currentPiece != NULL ) {
        free( gw->currentPiece );
    }

    gw->currentPiece = gw->nextPieceBox->nextPiece;

    Piece* newPiece = (Piece*) malloc( sizeof( Piece ) );
    newPiece->line = line;
    newPiece->column = column;
    newPiece->movementCount = 0;
    newPiece->maxCount = 60;
    newPiece->stopped = false;
    newPiece->type = type;
    newPiece->currentFrame = 0;

    switch ( type ) {
        case H:
            memcpy( newPiece->data, tetrominoeH, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeHDimensions, 16 * sizeof( int ) );
            newPiece->color = BLUE;
            break;
        case IL:
            memcpy( newPiece->data, tetrominoeIL, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeILDimensions, 16 * sizeof( int ) );
            newPiece->color = DARKBLUE;
            break;
        case L:
            memcpy( newPiece->data, tetrominoeL, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeLDimensions, 16 * sizeof( int ) );
            newPiece->color = ORANGE;
            break;
        case SQ:
            memcpy( newPiece->data, tetrominoeSQ, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeSQDimensions, 16 * sizeof( int ) );
            newPiece->color = YELLOW;
            break;
        case S:
            memcpy( newPiece->data, tetrominoeS, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeSDimensions, 16 * sizeof( int ) );
            newPiece->color = GREEN;
            break;
        case Z:
            memcpy( newPiece->data, tetrominoeZ, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeZDimensions, 16 * sizeof( int ) );
            newPiece->color = RED;
            break;
        case T:
            memcpy( newPiece->data, tetrominoeT, 64 * sizeof( bool ) );
            memcpy( newPiece->dimensions, tetrominoeTDimensions, 16 * sizeof( int ) );
            newPiece->color = PURPLE;
            break;
    }

    gw->nextPieceBox->nextPiece = newPiece;

}

void createRandomPiece( GameWorld *gw ) {

    int line = 0;
    int column = 0;
    PieceType type = GetRandomValue( 1, 7 );
    //PieceType type = H;

    switch ( type ) {
        case H:  line = -1; column = 3; break;
        case IL: line = -2; column = 3; break;
        case L:  line = -2; column = 3; break;
        case SQ: line = -2; column = 4; break;
        case S:  line = -2; column = 3; break;
        case Z:  line = -2; column = 3; break;
        case T:  line = -2; column = 3; break;
    }

    createPiece( type, line, column, gw );

}

void updatePiece( Piece *piece ) {

    if ( !piece->stopped ) {
        piece->movementCount++;
        if ( piece->movementCount == piece->maxCount ) {
            piece->line++;
            piece->movementCount = 0;
        }
    }

}

void drawPiece( Piece *piece ) {

    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            if ( piece->data[piece->currentFrame][i][j] ) {
                DrawRectangle( 
                    (piece->column + j) * 40, 
                    (piece->line + i) * 40, 
                    40, 40, 
                    piece->color );
            }
        }
    }

}

void turnPiece( Piece *piece ) {
    piece->currentFrame++;
    piece->currentFrame %= 4;
}

void checkPieceBoundaries( Piece *piece ) {

    int columnOffset = piece->dimensions[piece->currentFrame][2];
    int pieceColumn = piece->column + columnOffset;
    int pieceWidth = piece->dimensions[piece->currentFrame][0];

    if ( pieceColumn + pieceWidth >= 10 ) {
        piece->column = 10 - pieceWidth - columnOffset;
    }
    if ( pieceColumn < 0 ) {
        piece->column = 0 - columnOffset;
    }

}

void stopWhenReachedBottom( Piece *piece ) {

    int pieceLine = piece->line + piece->dimensions[piece->currentFrame][3];
    int pieceHeight = piece->dimensions[piece->currentFrame][1];

    if ( pieceLine + pieceHeight >= 20 ) {
        pieceLine = 20 - pieceHeight;
        piece->stopped = true;
    }

}

void stopWhenCollide( Piece *piece, GameWorld *gw ) {

    int line;
    int column;
    bool colide = false;
    int maxLine = -1000;

    for ( int i = 0; i < 4; i++ ) {
        line = piece->line + i;
        for ( int j = 0; j < 4; j++ ) {
            column = piece->column + j;
            if ( piece->data[piece->currentFrame][i][j] && 
                 line >= 0 && line < 20 && 
                 column >= 0 && column < 10 &&
                 gw->board[line][column] != 0 ) {
                colide = true;
                if ( maxLine < piece->line ) {
                    maxLine = piece->line;
                }
            }
        }
    }

    if ( colide ) {
        piece->line = maxLine - 1;
        piece->stopped = true;
    }

}

void copyPieceToBoard( Piece *piece, GameWorld *gw ) {

    int line;
    int column;

    for ( int i = 0; i < 4; i++ ) {
        line = piece->line + i;
        for ( int j = 0; j < 4; j++ ) {
            column = piece->column + j;
            if ( piece->data[piece->currentFrame][i][j] && 
                 line >= 0 && line < 20 && 
                 column >= 0 && column < 10 ) {
                gw->board[line][column] = piece->type;
            }
        }
    }

}

Color getColorByPieceType( PieceType type ) {
    
    switch ( type ) {
        case H: return BLUE;
        case IL: return DARKBLUE;
        case L: return ORANGE;
        case SQ: return YELLOW;
        case S: return GREEN;
        case Z: return RED;
        case T: return PURPLE;
        default: return BLACK;
    }

}

void processBoardToScore( GameWorld *gw ) {

    bool remove;

    for ( int i = 0; i < 20; i++ ) {
        remove = true;
        for ( int j = 0; j < 10; j++ ) {
            if ( gw->board[i][j] == 0 ) {
                remove = false;
                break;
            }
        }
        if ( remove ) {
            gw->scoreBox->points += 100;
            gw->linesBox->lines++;
            for ( int j = 0; j < 10; j++ ) {
                gw->board[i][j] = 0;
            }
        }
    }

}

void drawScoreBox( ScoreBox *score ) {

    char scoreLabel[10] = "Score";
    int scoreLabelSize = 20;

    char scorePoints[9];
    int scorePointsSize = 30;
    sprintf( scorePoints, "%08d", score->points );

    Rectangle r = { 
        .x = score->x, 
        .y = score->y, 
        .width = score->width, 
        .height = score->height
    };

    DrawRectangleRoundedLines( r, 0.5, 1, 2, BLACK );
    DrawRectangle( score->x + 25, score->y - 10, 
        MeasureText( scoreLabel, scoreLabelSize ) + 10, 
        20, BACKGROUND_COLOR );

    int w = MeasureText( scorePoints, scorePointsSize );

    DrawText( scoreLabel, score->x + 30, score->y - 10, scoreLabelSize, BLACK );
    DrawText( scorePoints, score->x + score->width / 2 - w / 2, score->y + 20, scorePointsSize, BLACK );

}

void drawLinesBox( LinesBox *lines ) {

    char linesLabel[10] = "Lines";
    int linesLabelSize = 20;

    char linesPoints[6];
    int linesPointsSize = 30;
    sprintf( linesPoints, "%05d", lines->lines );

    Rectangle r = { 
        .x = lines->x, 
        .y = lines->y, 
        .width = lines->width, 
        .height = lines->height
    };

    DrawRectangleRoundedLines( r, 0.5, 1, 2, BLACK );
    DrawRectangle( lines->x + 25, lines->y - 10, 
        MeasureText( linesLabel, linesLabelSize ) + 10, 
        20, BACKGROUND_COLOR );

    int w = MeasureText( linesPoints, linesPointsSize );

    DrawText( linesLabel, lines->x + 30, lines->y - 10, linesLabelSize, BLACK );
    DrawText( linesPoints, lines->x + lines->width / 2 - w / 2, lines->y + 20, linesPointsSize, BLACK );

}

void drawNextPieceBox( NextPieceBox *box ) {

    int tileLength = 30;
    Piece *piece = box->nextPiece;
    char nextPieceLabel[10] = "Next";
    int nextPieceLabelSize = 20;

    Rectangle r = { 
        .x = box->x, 
        .y = box->y, 
        .width = box->width, 
        .height = box->height
    };

    DrawRectangleRoundedLines( r, 0.3, 1, 2, BLACK );
    DrawRectangle( box->x + 25, box->y - 10, 
        MeasureText( nextPieceLabel, nextPieceLabelSize ) + 10, 
        20, BACKGROUND_COLOR );

    DrawText( nextPieceLabel, box->x + 30, box->y - 10, nextPieceLabelSize, BLACK );

    // draw piece (small)
    if ( piece != NULL ) {

        int pieceWidth = piece->dimensions[piece->currentFrame][0];
        int pieceHeight = piece->dimensions[piece->currentFrame][1];
        int lineOffset = piece->dimensions[piece->currentFrame][3];
        lineOffset = lineOffset > 0 ? lineOffset + 1 : lineOffset;

        int xStart = ( box->x + box->width / 2 ) - ( pieceWidth * tileLength ) / 2;
        int yStart = ( box->y + box->height / 2 ) - ( pieceHeight * tileLength ) / 2 - ( lineOffset * tileLength ) / 2;

        for ( int i = 0; i < 4; i++ ) {
            for ( int j = 0; j < 4; j++ ) {
                if ( piece->data[piece->currentFrame][i][j] ) {
                    DrawRectangle( 
                        xStart + j * tileLength, 
                        yStart + i * tileLength, 
                        tileLength, tileLength, 
                        BLACK );
                    DrawRectangle( 
                        xStart + j * tileLength, 
                        yStart + i * tileLength, 
                        tileLength - 2, tileLength - 2, 
                        piece->color );
                }
            }
        }

    }

}