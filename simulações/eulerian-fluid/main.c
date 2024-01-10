/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Base template for game
 * development in C using Raylib (https://www.raylib.com/).
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
typedef struct GameWorld {
    int dummy;
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


float simHeight;
float cScale;
float simWidth;
int cnt;

typedef enum FieldType {
    U_FIELD,
    V_FIELD,
    S_FIELD
} FieldType;

typedef struct Fluid {
    float density;
    int numX;
    int numY;
    int numCells;
    int h;
    float *u;
    float *v;
    float *newU;
    float *newV;
    float *p;
    float *s;
    float *m;
    float *newM;
} Fluid;

Fluid fluid;

float cX( float x ) {
    return x * cScale;
}

float cY( float y ) {
    return GetScreenHeight() - y * cScale;
}

int main( void ) {

    const int screenWidth = 400;
    const int screenHeight = 400;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Window Title" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    simHeight = 1.1;	
	cScale = GetScreenHeight() / simHeight;
	simWidth = GetScreenWidth() / cScale;
    cnt = 0;

    fluid = (Fluid) {
        
    };
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

}


void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    

    EndDrawing();

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    gw = (GameWorld) {
        .dummy = 0
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