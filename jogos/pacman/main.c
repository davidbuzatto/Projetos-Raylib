/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Pacman in C using Raylib (https://www.raylib.com/).
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
 * Project headers.
 --------------------------------------------*/
#include <utils.h>

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Point2 {
    int x;
    int y;
} Point2;

typedef struct Map {
    int graph[100][4];
    Point2 vertices[100];
    int verticesQuantity;
} Map;

typedef struct GameWorld {
    Map *map;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Map map;


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

void parseMapFile( const char *mapPath, Map *map );
void drawMap( const Map *map );

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

    const int screenWidth = 800;
    const int screenHeight = 800;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Pacman" );
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

    drawMap( gw->map );

    EndDrawing();

}

void drawMap( const Map *map ) {

    for ( int i = 0; i < map->verticesQuantity; i++ ) {
        const Point2 *p = &map->vertices[i];
        DrawCircle( 20 + p->x * 20, 20 + p->y * 20, 5, GREEN );
    }

}

void parseMapFile( const char *mapPath, Map *map ) {

    char *data = LoadFileText( mapPath );
    int currentLine = 0;
    int currentColumn = 0;

    while ( *data != '\0' ) {

        char c = *data;

        if ( c == '\n' ) {
            currentLine++;
            currentColumn = 0;
        } else {
            if ( c == 'v' ) {
                map->vertices[map->verticesQuantity++] = (Point2) {
                    .x = currentColumn,
                    .y = currentLine
                };
            }
            currentColumn++;
        }

        data++;

    }

    for ( int i = 0; i < map->verticesQuantity; i++ ) {
        Point2 *p = &map->vertices[i];
        int line = p->y;
        int column = p->x;
    }

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    map = (Map) {
        .graph = {0},
        .vertices = {0},
        .verticesQuantity = 0
    };

    parseMapFile( "resources/map.txt", &map );

    gw = (GameWorld) {
        .map = &map
    };

}

void destroyGameWorld( void ) {
    printf( "destroying game world...\n" );
}

void loadResources( void ) {
    printf( "loading resources...\n" );
}

void unloadResources( void ) {
    printf( "unloading resources...\n" );
}