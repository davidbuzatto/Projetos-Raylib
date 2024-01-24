/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief and simulation in C using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/*---------------------------------------------
 * Library headers.
 --------------------------------------------*/
#include <raylib.h>
#include <raymath.h>

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include <utils.h>

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int CELL_WIDTH = 2;
const unsigned int BACKGROUND_COLOR = 0x000000ff;
const unsigned int GRID_COLOR = 0xccccccff;
const bool DRAW_GRID = false;
const int SAND_LIMIT = 10;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct GameWorld {
    int lines;
    int columns;
    unsigned int *grid;
    bool drawGrid;
    int sandLimit;
    Color backgroundColor;
    Color gridColor;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Color currentColor;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( GameWorld *gw );
void move( int line, int column, GameWorld *gw );
void swap( int line, int column, int toLine, int toColumn, GameWorld *gw );
bool isLineColumnOk( int line, int column, GameWorld *gw );
void createSand( int line, int column, int limit, GameWorld *gw );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( const GameWorld *gw );

/**
 * @brief Create the global Game World object and all of its dependecies.
 */
void createGameWorld( void );

/**
 * @brief Destroy the global Game World object and all of its dependencies.
 */
void destroyGameWorld( void );

/**
 * @brief Load game resources like images, textures, sounds,
 * fonts, shaders etc.
 */
void loadResources( void );

/**
 * @brief Unload the once loaded game resources.
 */
void unloadResources( void );

int main( void ) {

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Simulação de Areia" );
    InitAudioDevice();
    SetTargetFPS( 120 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        /*currentColor = (Color) {
            .r = GetRandomValue( 100, 255 ),
            .g = GetRandomValue( 100, 255 ),
            .b = GetRandomValue( 100, 255 ),
            .a = 255
        };*/
        currentColor = ColorFromHSV( 
            GetRandomValue( 0, 55 ), 
            GetRandomValue( 1000, 2000 ) / 2000.0, 
            GetRandomValue( 1700, 2000 ) / 2000.0
        );
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        int line = GetMouseY() / CELL_WIDTH;
        int column = GetMouseX() / CELL_WIDTH;
        if ( isLineColumnOk( line, column, gw ) ) {
            int p = line * gw->columns + column;
            if ( gw->grid[p] == BACKGROUND_COLOR ) {
                //gw->grid[p] = ColorToInt( currentColor );
                createSand( line, column, gw->sandLimit, gw );
            }
        }
    }

    for ( int i = gw->lines-1; i >= 0; i-- ) {
        for ( int j = gw->columns-1; j >= 0; j-- ) {
            int p = i * gw->columns + j;
            if ( gw->grid[p] != BACKGROUND_COLOR ) {
                move( i, j, gw );
            }
        }
    }

}

void move( int line, int column, GameWorld *gw ) {

    int nextLine = line + 1;
    int nextColumn = column;
    int pNext = nextLine * gw->columns + nextColumn;

    if ( isLineColumnOk( nextLine, nextColumn, gw ) ) {
        if ( gw->grid[pNext] == BACKGROUND_COLOR ) {
            swap( line, column, nextLine, nextColumn, gw );
        } else {
            if ( GetRandomValue( 0, 1 ) == 0 ) {
                nextColumn--;
            } else {
                nextColumn++;
            }
            pNext = nextLine * gw->columns + nextColumn;
            if ( isLineColumnOk( nextLine, nextColumn, gw ) ) {
                if ( gw->grid[pNext] == BACKGROUND_COLOR ) {
                    swap( line, column, nextLine, nextColumn, gw );
                }
            }
        }
    }

}

void swap( int line, int column, int toLine, int toColumn, GameWorld *gw ) {
    int p1 = line * gw->columns + column;
    int p2 = toLine * gw->columns + toColumn;
    unsigned int t = gw->grid[p1];
    gw->grid[p1] = gw->grid[p2];
    gw->grid[p2] = t;
}

bool isLineColumnOk( int line, int column, GameWorld *gw ) {
    return line >= 0 && line < gw->lines && column >= 0 && column < gw->columns;
}

void createSand( int line, int column, int limit, GameWorld *gw ) {

    for ( int i = line - limit; i < line + limit + 1; i++ ) {
        for ( int j = column - limit; j < column + limit + 1; j++ ) {
            if ( isLineColumnOk( i, j, gw ) ) {
                if ( GetRandomValue( 0, 10 ) == 0 ) {
                    gw->grid[i*gw->columns+j] = ColorToInt( currentColor );
                }
            }
        }
    }

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( gw->backgroundColor );

    for ( int i = 0; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            int p = i * gw->columns + j;
            if ( gw->grid[p] != BACKGROUND_COLOR ) {
                DrawRectangle( j * CELL_WIDTH, i * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, GetColor( gw->grid[p] ) );
            }
        }
    }

    if ( gw->drawGrid ) {
        for ( int i = 1; i < gw->lines; i++ ) {
            DrawLine( 0, i * CELL_WIDTH, GetScreenWidth(), i * CELL_WIDTH, gw->gridColor );
        }
        for ( int i = 1; i < gw->columns; i++ ) {
            DrawLine( i * CELL_WIDTH, 0, i * CELL_WIDTH, GetScreenHeight(), gw->gridColor );
        }
    }

    EndDrawing();

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    gw = (GameWorld) {
        .lines = SCREEN_HEIGHT / CELL_WIDTH,
        .columns = SCREEN_WIDTH / CELL_WIDTH,
        .grid = NULL,
        .drawGrid = DRAW_GRID,
        .sandLimit = SAND_LIMIT,
        .backgroundColor = GetColor( BACKGROUND_COLOR ),
        .gridColor = GetColor( GRID_COLOR )
    };

    gw.grid = (unsigned int*) malloc( gw.lines * gw.columns * sizeof( unsigned int ) );

    for ( int i = 0; i < gw.lines; i++ ) {
        for ( int j = 0; j < gw.columns; j++ ) {
            gw.grid[i*gw.columns+j] = BACKGROUND_COLOR;
        }
    }

}

void destroyGameWorld( void ) {
    printf( "destroying game world...\n" );
    free( gw.grid );
}

void loadResources( void ) {
    printf( "loading resources...\n" );
}

void unloadResources( void ) {
    printf( "unloading resources...\n" );
}