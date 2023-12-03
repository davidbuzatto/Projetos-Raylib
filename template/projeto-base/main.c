/*******************************************************************************
 * Base template for simple game development using Raylib.
 * https://www.raylib.com/
 * 
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

// macros


// enums, structs, unions and custom types
typedef struct {
    int dummy;
} GameWorld;

// global variables


// function prototypes
void input( GameWorld *gw );
void update( GameWorld *gw );
void draw( GameWorld *gw );

int main( void ) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    GameWorld gw = {
        .dummy = 0
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT ); // turn antialiasing on (if possible)
    InitWindow( screenWidth, screenHeight, "Window Title" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        input( &gw );
        update( &gw );
        draw( &gw );
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void input( GameWorld *gw ) {

}

void update( GameWorld *gw ) {

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    DrawText( "Base Template", 100, 100, 40, BLACK );

    EndDrawing();

}