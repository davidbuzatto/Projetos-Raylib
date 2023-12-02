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

#include "main.h"

// macros


// enums, structs, unions and custom types
typedef struct {
    Player *player;
    Stage *stage;
    Camera2D *camera;
} GameWorld;

// global variables


// function prototypes
void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

int main( void ) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    Player player = {
        .data = { 
            .x = 100, 
            .y = 330, 
            .width = 50, 
            .height = 50, 
            .vx = 0,
            .vy = 0,
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
            .baseColor = BLACK
        }
    };

    parseTerrain( &stage, "#                                                #\n"
                          "#        %%%%%%%%%%                              #\n"
                          "#                                                #\n"
                          "#                                                #\n"
                          "#                                                #\n"
                          "#                                                #\n"
                          "#                                                #\n"
                          "#             %%%%                               #\n"
                          "#                                                #\n"
                          "#                                                #\n"
                          "##          %%%%%%%%                             #\n"
                          "##                                               #\n"
                          "#####                     ################       #\n"
                          "##################################################\n"
                          "##################################################" );

    Camera2D camera = { 0 };
    camera.target = (Vector2){ player.data.x, player.data.y - 150 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    GameWorld gw = {
        .player = &player,
        .stage = &stage,
        .camera = &camera
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "RayMario");
    //InitAudioDevice();
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    //CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {
    inputAndUpdatePlayer( gw->player, gw->stage );
    inputAndUpdateStage( gw->stage );
    gw->camera->target = (Vector2){ gw->player->data.x, gw->player->data.y - 150 };
}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( gw->stage->data.baseColor );

    BeginMode2D( *(gw->camera) );
    drawStage( gw->stage );
    drawPlayer( gw->player );
    EndMode2D();

    EndDrawing();

}