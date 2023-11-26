/*******************************************************************************
 * Mario clone written using Raylib (https://www.raylib.com/)
 * 
 * Main file.
 * 
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "base.h"
#include "stage.h"
#include "player.h"

// macros


// enums, structs, unions and custom types
typedef struct {
    Player *player;
    Stage *stage;
} GameWorld;

// global variables


// function prototypes
void input( GameWorld *gw );
void update( GameWorld *gw );
void draw( GameWorld *gw );

int main( void ) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    Player player = {
        .data = { 
            .x = 100, 
            .y = 100, 
            .width = 100, 
            .height = 100, 
            .vx = 0,
            .vy = 5,
            .baseColor = BLUE
        },
        .state = PLAYER_STATE_ON_GROUND
    };

    Stage stage = {
        .data = { 
            .x = 0, 
            .y = 500, 
            .width = 800, 
            .height = 50, 
            .baseColor = GRAY
        }
    };
    parseTerrain( &stage, "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "                    \n"
                          "#                   \n"
                          "#             %%%   \n"
                          "##                  \n"
                          "##          %%%%    \n"
                          "#####               \n"
                          "####################" );

    GameWorld gw = {
        .player = &player,
        .stage = &stage
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT ); // turn antialiasing on (if possible)
    InitWindow( screenWidth, screenHeight, "RayMario");
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
    inputPlayer( gw->player, gw->stage );
}

void update( GameWorld *gw ) {
    updateStage( gw->stage );
    updatePlayer( gw->player, gw->stage );
}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( RAYWHITE );

    drawStage( gw->stage );
    drawPlayer( gw->player );

    EndDrawing();

}