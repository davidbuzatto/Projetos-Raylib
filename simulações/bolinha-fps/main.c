/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Ball simulation C using Raylib (https://www.raylib.com/).
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
 const int SIZE_X = 100;
 const int SIZE_Y = 100;
 const int TARGET_FPS = 60;

 /*const int VEL_X = 600;
 const int VEL_Y = 600;
 const int GRAVITY_Y = 900;*/
 
 const int VEL_X = 200;
 const int VEL_Y = 100;
 const int GRAVITY_Y = 200;


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Ball {
    Vector2 pos;
    Vector2 vel;
    float friction;
    float elasticity;
    Vector2 gravity;
    float radius;
    Color color;
} Ball;

typedef struct GameWorld {
    Ball *ball;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
Ball ball;
GameWorld gw;
float deltaTime;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( GameWorld *gw, float delta );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( const GameWorld *gw );

void drawBall( const Ball *ball );

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
    const int screenHeight = 600;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Bolinha" );
    InitAudioDevice();
    SetTargetFPS( TARGET_FPS );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw, deltaTime );
        draw( &gw );
        deltaTime = GetFrameTime();
    }
    destroyGameWorld();
    unloadResources();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw, float deltaTime ) {

    Ball *ball = gw->ball;

    ball->pos.x += ball->vel.x * deltaTime;
    ball->pos.y += ball->vel.y * deltaTime;

    if ( ball->pos.x + ball->radius > GetScreenWidth() ) {
        ball->vel.x = -ball->vel.x * ball->elasticity;
        ball->pos.x = GetScreenWidth() - ball->radius;
    } else if ( ball->pos.x - ball->radius < 0 ) {
        ball->vel.x = -ball->vel.x * ball->elasticity;
        ball->pos.x = ball->radius;
    }

    if ( ball->pos.y + ball->radius > GetScreenHeight() ) {
        ball->vel.y = -ball->vel.y * ball->elasticity;
        ball->pos.y = GetScreenHeight() - ball->radius;
    } else if ( ball->pos.y - ball->radius < 0 ) {
        ball->vel.y = ball->vel.y * ball->elasticity;
        ball->pos.y = ball->radius;
    }

    float fx = ( ball->vel.x * ball->friction ) * deltaTime;
    float fy = ( ball->vel.y * ball->friction ) * deltaTime;

    ball->vel.x = ball->vel.x - fx;
    ball->vel.y = ball->vel.y + ( ball->gravity.y * deltaTime ) - fy;

    /*ball->vel.x = ball->vel.x * ball->friction;
    ball->vel.y = ball->vel.y * ball->friction + ( ball->gravity.y * deltaTime );*/

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = SIZE_X; i < GetScreenWidth(); i += SIZE_X ) {
        DrawLineEx( (Vector2){i, 0}, (Vector2){i, 10}, 2, BLACK );
        DrawLine( i, 0, i, GetScreenHeight(), GRAY );
        DrawText( TextFormat( "%d", i ), i + 5, 15, 10, BLACK );
    }
    for ( int i = SIZE_Y; i < GetScreenHeight(); i += SIZE_Y ) {
        if ( i != 0 ) {
            DrawLineEx( (Vector2){0, i}, (Vector2){10, i}, 2, BLACK );
            DrawLine( 0, i, GetScreenWidth(), i, GRAY );
            DrawText( TextFormat( "%d", i ), 15, i - 15, 10, BLACK );
        }
    }
    drawBall( gw->ball );

    DrawFPS( 20, 20 );
    DrawText( TextFormat( "delta time: %.2f", deltaTime ), 20, 40, 20, BLACK );
    DrawText( TextFormat( "ellapsed time: %.2f", GetTime() ), 20, 60, 20, BLACK );

    EndDrawing();

}

void drawBall( const Ball *ball ) {

    DrawLineV( ball->pos, (Vector2){ ball->pos.x, 0 }, ball->color );
    DrawLineV( ball->pos, (Vector2){ 0, ball->pos.y }, ball->color );
    DrawCircleV( ball->pos, ball->radius, ball->color );

    DrawText( TextFormat( "(%.2f, %.2f)", ball->pos.x, ball->pos.y ), 
                          ball->pos.x + ball->radius + 10, ball->pos.y - 10, 
                          10, BLUE );

    Vector2 vVel = {
        ball->pos.x + ball->vel.x,
        ball->pos.y + ball->vel.y,
    };

    Vector2 vGrav = {
        ball->pos.x + ball->gravity.x,
        ball->pos.y + ball->gravity.y,
    };

    DrawLineEx( 
        ball->pos, 
        (Vector2){
            ball->pos.x + ball->vel.x,
            ball->pos.y + ball->vel.y,
        }, 2, ORANGE );    
    DrawPoly( 
        vVel, 3, 10, 
        toDegrees( 
            atan2( 
                vVel.y - ball->pos.y, 
                vVel.x - ball->pos.x
            )
        ), ORANGE );
    
    DrawLineEx( 
        ball->pos, 
        vGrav, 2, LIME );
    DrawPoly( 
        vGrav, 3, 10, 
        toDegrees( 
            atan2( 
                vGrav.y - ball->pos.y, 
                vGrav.x - ball->pos.x
            )
        ), LIME );

}

void createGameWorld( void ) {

    deltaTime = 0;

    ball = (Ball) {
        .pos = {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2
        },
        .vel = {
            .x = VEL_X,
            .y = VEL_Y
        },
        .friction = 0.999,
        .elasticity = 0.9,
        .gravity = {
            .x = 0,
            .y = GRAVITY_Y
        },
        .radius = 20,
        .color = BLUE
    };

    gw = (GameWorld) {
        .ball = &ball
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