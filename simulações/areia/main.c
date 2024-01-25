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

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#undef RAYGUI_IMPLEMENTATION

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
const unsigned int GRID_BACKGROUND_COLOR = 0x000000ff;
const unsigned int GRID_COLOR = 0xccccccff;
const bool DRAW_GRID = false;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Grain {
    unsigned int color;
    float velY;
} Grain;

typedef struct GameWorld {
    int lines;
    int columns;
    Grain *grid;
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

float sliderColor1 = 0.0f;
float sliderColor2 = 55.0f;
float sliderSat = 1.0f;
float sliderVal = 1.0f;
float sliderLimit = 10.0f;
float sliderInitialVelY = 0.0f;
float sliderGravity = 0.1f;

Rectangle sliderColor1Rect = { 50, 20, 200, 20 };
Rectangle sliderColor2Rect = { 50, 50, 200, 20 };
Rectangle sliderSatRect = { 50, 80, 200, 20 };
Rectangle sliderValRect = { 50, 110, 200, 20 };
Rectangle sliderLimitRect = { 400, 20, 200, 20 };
Rectangle sliderInitialVelYRect = { 400, 50, 200, 20 };
Rectangle sliderGravityRect = { 400, 80, 200, 20 };

bool draggingSliders = false;
bool showControls = true;


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
void createSand( int line, int column, int limit, float initialVelY, GameWorld *gw );

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
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    gw->sandLimit = sliderLimit;

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {

        Vector2 p = GetMousePosition();

        if ( showControls &&
             ( CheckCollisionPointRec( p, sliderColor1Rect ) || 
               CheckCollisionPointRec( p, sliderColor2Rect ) || 
               CheckCollisionPointRec( p, sliderSatRect ) || 
               CheckCollisionPointRec( p, sliderValRect ) || 
               CheckCollisionPointRec( p, sliderLimitRect ) || 
               CheckCollisionPointRec( p, sliderInitialVelYRect ) || 
               CheckCollisionPointRec( p, sliderGravityRect ) ) ) {
            draggingSliders = true;
        } else {
            draggingSliders = false;
            currentColor = ColorFromHSV( 
                GetRandomValue( fmin( sliderColor1, sliderColor2 ), fmax( sliderColor1, sliderColor2 ) ), 
                sliderSat, 
                sliderVal
            );
        }

    }

    if ( !draggingSliders ) {
        if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
            int line = GetMouseY() / CELL_WIDTH;
            int column = GetMouseX() / CELL_WIDTH;
            if ( isLineColumnOk( line, column, gw ) ) {
                int p = line * gw->columns + column;
                if ( gw->grid[p].color == GRID_BACKGROUND_COLOR ) {
                    createSand( line, column, gw->sandLimit, sliderInitialVelY, gw );
                }
            }
        }
    }

    for ( int i = gw->lines-1; i >= 0; i-- ) {
        for ( int j = gw->columns-1; j >= 0; j-- ) {
            int p = i * gw->columns + j;
            if ( gw->grid[p].color != GRID_BACKGROUND_COLOR && gw->grid[p].velY != 0 ) {
                move( i, j, gw );
            }
        }
    }

    for ( int i = gw->lines-1; i >= 0; i-- ) {
        for ( int j = gw->columns-1; j >= 0; j-- ) {
            int p = i * gw->columns + j;
            if ( gw->grid[p].color != GRID_BACKGROUND_COLOR ) {
                gw->grid[p].velY += sliderGravity;
            }
        }
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        showControls = !showControls;
    }

}

void move( int line, int column, GameWorld *gw ) {

    Grain *g = &gw->grid[line*gw->columns+column];
    int nextLine = line + (int) g->velY;
    int nextColumn = column;
    int pNext = nextLine * gw->columns + nextColumn;

    if ( isLineColumnOk( nextLine, nextColumn, gw ) ) {

        if ( gw->grid[pNext].color == GRID_BACKGROUND_COLOR ) {
            swap( line, column, nextLine, nextColumn, gw );
        } else {
            if ( GetRandomValue( 0, 1 ) == 0 ) {
                nextColumn--;
            } else {
                nextColumn++;
            }
            nextLine = line + 1;
            pNext = nextLine * gw->columns + nextColumn;
            if ( isLineColumnOk( nextLine, nextColumn, gw ) ) {
                if ( gw->grid[pNext].color == GRID_BACKGROUND_COLOR ) {
                    swap( line, column, nextLine, nextColumn, gw );
                }/* else {
                    gw->grid[pNext].velY = 0;
                }*/
            }
        }
    }

}

void swap( int line, int column, int toLine, int toColumn, GameWorld *gw ) {
    int p1 = line * gw->columns + column;
    int p2 = toLine * gw->columns + toColumn;
    Grain g = gw->grid[p1];
    gw->grid[p1] = gw->grid[p2];
    gw->grid[p2] = g;
}

bool isLineColumnOk( int line, int column, GameWorld *gw ) {
    return line >= 0 && line < gw->lines && column >= 0 && column < gw->columns;
}

void createSand( int line, int column, int limit, float initialVelY, GameWorld *gw ) {

    for ( int i = line - limit; i < line + limit + 1; i++ ) {
        for ( int j = column - limit; j < column + limit + 1; j++ ) {
            if ( isLineColumnOk( i, j, gw ) ) {
                if ( GetRandomValue( 0, 10 ) == 0 ) {
                    gw->grid[i*gw->columns+j].color = ColorToInt( currentColor );
                    gw->grid[i*gw->columns+j].velY = initialVelY;
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
            if ( gw->grid[p].color != GRID_BACKGROUND_COLOR ) {
                DrawRectangle( j * CELL_WIDTH, i * CELL_WIDTH, CELL_WIDTH, CELL_WIDTH, GetColor( gw->grid[p].color ) );
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

    if ( showControls ) {

        GuiSlider( sliderColor1Rect, "Cor 1:", TextFormat("%2.2f", sliderColor1), &sliderColor1, 0, 360 );
        GuiSlider( sliderColor2Rect, "Cor 2:", TextFormat("%2.2f", sliderColor2), &sliderColor2, 0, 360 );
        GuiSlider( sliderSatRect, "Sat:", TextFormat("%2.2f", sliderSat), &sliderSat, 0, 1 );
        GuiSlider( sliderValRect, "Val:", TextFormat("%2.2f", sliderVal), &sliderVal, 0, 1 );

        GuiSlider( sliderLimitRect, "Quantidade:", TextFormat("%2.2f", sliderLimit), &sliderLimit, 1, 20 );
        GuiSlider( sliderInitialVelYRect, "Vel. Y:", TextFormat("%2.2f", sliderInitialVelY), &sliderInitialVelY, 0, 2 );
        GuiSlider( sliderGravityRect, "Gravidade:", TextFormat("%2.2f", sliderGravity), &sliderGravity, 0, 2 );

        // color bar
        int q = 10;
        int t = sliderColor1Rect.width / q;
        int hueStart = sliderColor1 < sliderColor2 ? sliderColor1 : sliderColor2;
        int hueEnd = sliderColor1 < sliderColor2 ? sliderColor2 : sliderColor1;
        int hueTick = abs( hueEnd - hueStart ) / q;

        for ( int i = 0; i < q; i++ ) {
            Color c1 = ColorFromHSV( hueStart + hueTick * i, sliderSat, sliderVal );
            Color c2 = ColorFromHSV( hueStart + hueTick * (i+1), sliderSat, sliderVal );
            DrawRectangleGradientH( 50 + i * t, 140, t, 20, c1, c2 );
        }
        DrawRectangleLines( 50, 140, t * q, 20, WHITE );

        Vector2 vC1 = { 
            sliderColor1Rect.x + sliderColor1Rect.width + 40, 
            sliderColor1Rect.y
        };
        Vector2 vC2 = { 
            sliderColor2Rect.x + sliderColor2Rect.width + 40, 
            sliderColor2Rect.y
        };
        Vector2 size = { 20, 20 };

        DrawRectangleV( vC1, size, 
                        ColorFromHSV( sliderColor1, sliderSat, sliderVal ) );
        DrawRectangleV( vC2, size,
                       ColorFromHSV( sliderColor2, sliderSat, sliderVal ) );
        DrawRectangleLines( vC1.x, vC1.y, size.x, size.y, WHITE );
        DrawRectangleLines( vC2.x, vC2.y, size.x, size.y, WHITE );

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
        .sandLimit = sliderLimit,
        .backgroundColor = GetColor( GRID_BACKGROUND_COLOR ),
        .gridColor = GetColor( GRID_COLOR )
    };

    gw.grid = (Grain*) malloc( gw.lines * gw.columns * sizeof( Grain ) );

    for ( int i = 0; i < gw.lines; i++ ) {
        for ( int j = 0; j < gw.columns; j++ ) {
            gw.grid[i*gw.columns+j].color = GRID_BACKGROUND_COLOR;
            gw.grid[i*gw.columns+j].velY = 0;
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