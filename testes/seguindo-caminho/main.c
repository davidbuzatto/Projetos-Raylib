/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Base template for game
 * development in C using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2023
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
#include "include/raylib.h"

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include "include/utils.h"

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

float t = 0;
float inc = 0.01;
int xP1 = 100;
int yP1 = 100;
int xC2 = 300;
int yC2 = 100;
int xC3 = 500;
int yC3 = 700;
int xP4 = 700;
int yP4 = 700;
bool controlActive = false;


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

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 800;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Follow the Path!" );
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

    if ( IsKeyPressed( KEY_C ) ) {
        controlActive = !controlActive;
    }
    
    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        if ( controlActive || IsKeyDown( KEY_LEFT_CONTROL ) ) {
            xC2 = GetMouseX();
            yC2 = GetMouseY();
        } else {
            xP1 = GetMouseX();
            yP1 = GetMouseY();
        }
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) ) {
        if ( controlActive || IsKeyDown( KEY_LEFT_CONTROL ) ) {
            xC3 = GetMouseX();
            yC3 = GetMouseY();
        } else {
            xP4 = GetMouseX();
            yP4 = GetMouseY();
        }
    }

    t += inc;
    if ( t < 0 || t > 1 ) {
        inc = -inc;
        t += inc;
    }

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    Vector2 p1 = {
        .x = xP1,
        .y = yP1
    };

    Vector2 c2 = {
        .x = xC2,
        .y = yC2
    };

    Vector2 c3 = {
        .x = xC3,
        .y = yC3
    };

    Vector2 p4 = {
        .x = xP4,
        .y = yP4
    };

    DrawCircleV( p1, 4, DARKGRAY );
    DrawCircleV( c2, 4, DARKGRAY );
    DrawCircleV( c3, 4, DARKGRAY );
    DrawCircleV( p4, 4, DARKGRAY );

    DrawSplineSegmentLinear( p1, p4, 2, MAROON );
    DrawSplineSegmentBasis( p1, c2, c3, p4, 2, DARKBLUE );
    DrawSplineSegmentCatmullRom( p1, c2, c3, p4, 2, DARKGREEN );
    DrawSplineSegmentBezierQuadratic( p1, c2, p4, 2, DARKPURPLE );
    DrawSplineSegmentBezierCubic( p1, c2, c3, p4, 2, ORANGE );

    Vector2 pL = GetSplinePointLinear( p1, p4, t );
    Vector2 pB = GetSplinePointBasis( p1, c2, c3, p4, t );
    Vector2 pCR = GetSplinePointCatmullRom( p1, c2, c3, p4, t );
    Vector2 pBQ = GetSplinePointBezierQuad( p1, c2, p4, t );
    Vector2 pBC = GetSplinePointBezierCubic( p1, c2, c3, p4, t ); 

    DrawCircleV( pL, 10, RED );
    DrawCircleV( pB, 10, BLUE );
    DrawCircleV( pCR, 10, LIME );
    DrawCircleV( pBQ, 10, VIOLET );
    DrawCircleV( pBC, 10, GOLD );

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