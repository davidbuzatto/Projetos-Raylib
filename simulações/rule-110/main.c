/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Rule 110 using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/*---------------------------------------------
 * Library headers.
 --------------------------------------------*/
#include <raylib.h>


/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const int GENERATIONS = 250;
const int TILE_WIDTH = 5;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct GameWorld {
    char *pattern;
    int patternLength;
    int generations;
    char *data;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( GameWorld *gw );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( const GameWorld *gw );

char rule110( char c1, char c2, char c3 );
void generate( GameWorld *gw );

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

    const int screenWidth = GENERATIONS * TILE_WIDTH;
    const int screenHeight = GENERATIONS * TILE_WIDTH;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Rule 110" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    destroyGameWorld();
    unloadResources();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    int lineLength = gw->patternLength;
    for ( int i = 0; i < gw->generations; i++ ) {
        for ( int j = 0; j < lineLength; j++ ) {
            char c = gw->data[i * lineLength + j];
            Color color;
            switch ( c ) {
                case '0': color = WHITE; break;
                case '1': color = BLACK; break;
                default: color = GREEN; break;
            }
            DrawRectangle( j * TILE_WIDTH, i * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, color );
        } 
    }

    EndDrawing();

}

char rule110( char c1, char c2, char c3 ) {
    
    if ( c1 == '1' ) {
        if ( c2 == '1' ) {
            if ( c3 == '1' ) {
                return '0';  // 111 0
            } else if ( c3 == '0' ) {
                return '1';  // 110 1
            }
        } else if ( c2 == '0' ) {
            if ( c3 == '1' ) {
                return '1';  // 101 1
            } else if ( c3 == '0' ) {
                return '0';  // 100 0
            }
        }
    } else if ( c1 == '0' ) {
        if ( c2 == '1' ) {
            if ( c3 == '1' ) {
                return '1';  // 011 1
            } else if ( c3 == '0' ) {
                return '1';  // 010 1
            }
        } else if ( c2 == '0' ) {
            if ( c3 == '1' ) {
                return '1';  // 001 1
            } else if ( c3 == '0' ) {
                return '0';  // 000 0
            }
        }
    }

    // should neve reach this point
    return '\0';

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    gw = (GameWorld) {
        .pattern = NULL,
        .patternLength = 0,
        .generations = GENERATIONS,
        .data = NULL
    };

    gw.patternLength = ( GENERATIONS + 2 ) * sizeof( char );
    gw.pattern = (char*) malloc( gw.patternLength );
    for ( int i = 0; i < gw.patternLength; i++ ) {
        if ( i == gw.patternLength-3 ) {
            gw.pattern[i] = '1';
        } else {
            gw.pattern[i] = '0';
        }
    }

    size_t size = gw.patternLength * gw.generations * sizeof( char );
    gw.data = (char*) malloc( size );
    
    generate( &gw );

}

void generate( GameWorld *gw ) {
    
    int lineLength = gw->patternLength;
    size_t size = gw->patternLength * gw->generations * sizeof( char );
    memset( gw->data, '0', size );

    for ( int i = 0; i < lineLength; i++ ) {
        gw->data[i] = gw->pattern[i%gw->patternLength];
    }

    for ( int i = 1; i < gw->generations; i++ ) {
        for ( int j = 0; j < lineLength; j++ ) {
            int p1 = j%lineLength;
            int p2 = (j+1)%lineLength;
            int p3 = (j+2)%lineLength;
            char c1 = gw->data[(i-1)*lineLength+p1];
            char c2 = gw->data[(i-1)*lineLength+p2];
            char c3 = gw->data[(i-1)*lineLength+p3];
            gw->data[i*lineLength+p2] = rule110( c1, c2, c3 );
        }
    }

}

void destroyGameWorld( void ) {

    printf( "destroying game world...\n" );

    free( gw.data );

}

void loadResources( void ) {
    printf( "loading resources...\n" );
}

void unloadResources( void ) {
    printf( "unloading resources...\n" );
}