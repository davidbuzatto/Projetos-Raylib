/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Arkanoid game in C using Raylib (https://www.raylib.com/).
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
#define TARGET_HORIZONTAL_MAX_QUANTITY 11
#define TARGET_VERTICAL_MAX_QUANTITY 22

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const int TARGET_WIDTH = 64;
const int TARGET_HEIGHT = 32;
const int PLAYER_AREA_HEIGHT = 160; // 5 targets

const int PLAYER_WIDTH = 128;
const int PLAYER_HEIGHT = 32;
const int BALL_RADIUS = 10;

const int GAME_STATUS_WIDTH = 224;

const int ARENA_MARGIN = 32;

const Color TARGET_COLORS[6] = {
    { 162, 160, 162, 255 },
    { 226, 57, 0, 255 },
    { 0, 84, 246, 255 },
    { 255, 149, 1, 255 },
    { 255, 119, 145, 255 },
    { 116, 183, 9, 255 },
};

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Player {
    Rectangle rec;
    Vector2 vel;
    Color color;
} Player;

typedef struct Target {
    Rectangle rec;
    Color color;
} Target;

typedef struct Ball {
    Vector2 pos;
    Vector2 vel;
    int radius;
    Color color;
} Ball;

typedef struct Arena {
    Rectangle rec;
    Rectangle battleField;
    Player *player;
    Target targets[TARGET_VERTICAL_MAX_QUANTITY][TARGET_HORIZONTAL_MAX_QUANTITY];
    int targetLines;
    int targetColumns;
    Ball *ball;
    Color color;
    Color battleFieldColor;
} Arena;

typedef struct Score {
    int score;
    int lives;
    int round;
} Score;

typedef struct GameWorld {
    Arena *arena;
    Score *score;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
Player player;
Target target;
Ball ball;
Arena arena;
Score score;
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

void drawArena( const Arena *arena );
void drawPlayer( const Player *player );
void drawTarget( const Target *target );
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

    const int screenWidth = ARENA_MARGIN + TARGET_WIDTH * TARGET_HORIZONTAL_MAX_QUANTITY + ARENA_MARGIN + GAME_STATUS_WIDTH;
    const int screenHeight = ARENA_MARGIN + TARGET_HEIGHT * TARGET_VERTICAL_MAX_QUANTITY;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Arkanoid" );
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
    ClearBackground( BLACK );

    drawArena( gw->arena );

    EndDrawing();

}

void drawArena( const Arena *arena ) {
    DrawRectangleRec( arena->rec, arena->color );
    DrawRectangleRec( arena->battleField, arena->battleFieldColor );
    for ( int i = 0; i < arena->targetLines; i++ ) {
        for ( int j = 0; j < arena->targetColumns; j++ ) {
            drawTarget( &arena->targets[i][j] );
        }
    }
    drawPlayer( arena->player );
    drawBall( arena->ball );
}

void drawPlayer( const Player *player ) {
    DrawRectangleRec( player->rec, player->color );
}

void drawTarget( const Target *target ) {
    DrawRectangleRec( target->rec, BLACK );
    DrawRectangle( target->rec.x, target->rec.y, target->rec.width - 4, target->rec.height - 4, target->color );
}

void drawBall( const Ball *ball ) {
    DrawCircle( ball->pos.x, ball->pos.y, ball->radius, ball->color );
}

void createGameWorld( void ) {

    int arenaWidth = TARGET_WIDTH * TARGET_HORIZONTAL_MAX_QUANTITY;
    int arenaHeight = TARGET_HEIGHT * TARGET_VERTICAL_MAX_QUANTITY;
    int vOffsetTargets = 4 * TARGET_HEIGHT;

    arena = (Arena) {
        .rec = {
            .x = 0,
            .y = 0,
            .width = arenaWidth + ARENA_MARGIN * 2,
            .height = arenaHeight + ARENA_MARGIN
        },
        .battleField = {
            .x = ARENA_MARGIN,
            .y = ARENA_MARGIN,
            .width = arenaWidth,
            .height = arenaHeight
        },
        .targets = {0},
        .targetLines = 6,
        .targetColumns = 11,
        .color = GRAY,
        .battleFieldColor = BLACK
    };

    player = (Player) {
        .rec = {
            .x = arena.battleField.x + arena.battleField.width / 2 - PLAYER_WIDTH / 2,
            .y = arena.battleField.height - PLAYER_AREA_HEIGHT + PLAYER_HEIGHT,
            .width = PLAYER_WIDTH,
            .height = PLAYER_HEIGHT
        },
        .vel = {
            .x = 1,
            .y = 0
        },
        .color = DARKGRAY
    };

    ball = (Ball) {
        .pos = {
            .x = arena.battleField.x + arena.battleField.width / 2,
            .y = player.rec.y - BALL_RADIUS
        },
        .vel = {
            .x = 1,
            .y = 1
        },
        .radius = BALL_RADIUS,
        .color = ORANGE
    };
    
    arena.player = &player;
    arena.ball = &ball;

    for ( int i = 0; i < arena.targetLines; i++ ) {
        for ( int j = 0; j < arena.targetColumns; j++ ) {
            arena.targets[i][j] = (Target) {
                .rec = {
                    .x = arena.battleField.x + j * TARGET_WIDTH,
                    .y = vOffsetTargets + arena.battleField.y + i * TARGET_HEIGHT,
                    .width = TARGET_WIDTH,
                    .height = TARGET_HEIGHT
                },
                .color = TARGET_COLORS[i]
            };
        }
    }

    score = (Score) {
        .score = 0,
        .lives = 3,
        .round = 1
    };

    gw = (GameWorld) {
        .arena = &arena,
        .score = &score
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