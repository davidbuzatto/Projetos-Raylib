/*******************************************************************************
 * Flappy Bird using Raylib (https://www.raylib.com/).
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
#include "include/utils.h"

#define MAX_ACTIVE_PIPES 20

const int GROUND_HEIGHT = 80;
const float GRAVITY = .5;

const int FLAPPY_BIRD_X_START = 250;
const int FLAPPY_BIRD_Y_START = 250;
const int FLAPPY_BIRD_WIDTH = 60;
const int FLAPPY_BIRD_HEIGHT = 42;
const float FLAPPY_BIRD_START_ATTACK_ANGLE = 0;
const float FLAPPY_BIRD_ATTACK_ANGLE_DECREMENT = -30;
const float FLAPPY_BIRD_ATTACK_ANGLE_INCREMENT = .7;
const float FLAPPY_BIRD_MIN_ATTACK_ANGLE = -30;
const float FLAPPY_BIRD_MAX_ATTACK_ANGLE = 70;
const float FLAPPY_BIRD_BASE_Y_VELOCITY = 0;
const float FLAPPY_BIRD_JUMP_SPEED = -12;

const int PIPE_COUPLE_X_START = 500;
const int PIPE_WIDE_WIDTH = 90;
const int PIPE_NARROW_WIDTH = 80;
const int PIPE_SLICE_HEIGHT = 40;
const int PIPE_COUPLE_NARROW_TOTAL_SLICES = 8;
const int PIPE_COUPLE_HORIZONTAL_GAP = 180;
const int PIPE_COUPLE_VERTICAL_GAP = 170;
const int PIPE_COUPLE_QUANTITY = 5;

typedef struct PolarCoord {
    float angle;
    float distance;
} PolarCoord;

typedef enum FlappyBirdState {
    FLAPPY_BIRD_STATE_IDLE,
    FLAPPY_BIRD_STATE_ALIVE,
    FLAPPY_BIRD_STATE_DEAD
} FlappyBirdState;

typedef struct FlappyBird {
    Vector2 pos;
    Vector2 vel;
    int width;
    int height;
    float jumpSpeed;
    float attackAngle;
    float minAttackAngle;
    float maxAttackAngle;
    Texture texture;
    Color color;
    FlappyBirdState state;
    PolarCoord collisionProbes[12];
} FlappyBird;

typedef struct PipeCouple {
    int x;
    int wideWidth;
    int narrowWidth;
    int sliceHeight;
    int narrowSlicesUp;
    int narrowTotalSlices;
    int verticalGap;
    Texture texture;
    Color color;
} PipeCouple;

typedef struct Ground {
    Vector2 pos;
    Vector2 vel;
    int width;
    int height;
    Texture texture;
    Color color;
} Ground;

typedef struct GameWorld {
    FlappyBird *fb;
    PipeCouple pcs[MAX_ACTIVE_PIPES];
    Ground *ground;
} GameWorld;

GameWorld gw;
FlappyBird fb;
Ground ground;

void inputAndUpdate( GameWorld *gw );
void draw( const GameWorld *gw );

void drawGround( const Ground *ground );
void drawFlappyBird( const FlappyBird *fb );
void drawPipe( const PipeCouple *pc );
void drawPipes( const PipeCouple *pcs );

void resetGame( GameWorld *gw );

void createGameWorld( void );
void destroyGameWorld( void );

int main( void ) {

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( 1200, 650, "Flappy Bird" );
    //InitAudioDevice();
    SetTargetFPS( 60 );    

    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    destroyGameWorld();

    //CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    FlappyBird *fb = gw->fb;
    PipeCouple *pcs = gw->pcs;
    Ground *ground = gw->ground;

    if ( fb->state == FLAPPY_BIRD_STATE_IDLE ) {

        if ( IsKeyPressed( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            fb->state = FLAPPY_BIRD_STATE_ALIVE;
        }

    } else if ( fb->state == FLAPPY_BIRD_STATE_ALIVE ) {

        if ( IsKeyPressed( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            fb->vel.y = fb->jumpSpeed;
            fb->attackAngle += FLAPPY_BIRD_ATTACK_ANGLE_DECREMENT;
        }

        fb->pos.y += fb->vel.y;
        fb->attackAngle += FLAPPY_BIRD_ATTACK_ANGLE_INCREMENT;

        if ( fb->pos.y - fb->height/2 < 0 ) {
            fb->pos.y = fb->height/2;
            fb->vel.y = FLAPPY_BIRD_BASE_Y_VELOCITY;
        } else if ( fb->pos.y + fb->height/2 > ground->pos.y ) {
            fb->pos.y = ground->pos.y - fb->height/2;
            fb->state = FLAPPY_BIRD_STATE_DEAD;
        }

        if ( fb->attackAngle < fb->minAttackAngle ) {
            fb->attackAngle = fb->minAttackAngle;
        } else if ( fb->attackAngle > fb->maxAttackAngle ) {
            fb->attackAngle = fb->maxAttackAngle;
        }

        fb->vel.y += GRAVITY;

    } else if ( fb->state == FLAPPY_BIRD_STATE_DEAD ) {

        if ( IsKeyPressed( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            resetGame( gw );
        }

    }

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawGround( gw->ground );
    drawFlappyBird( gw->fb );
    drawPipes( gw->pcs );

    EndDrawing();

}

void drawGround( const Ground *ground ) {
    DrawRectangle( 
        ground->pos.x, ground->pos.y, 
        ground->width, ground->height, 
        ground->color );
}

void drawFlappyBird( const FlappyBird *fb ) {
    
    DrawRectanglePro(
        (Rectangle){
            .x = fb->pos.x,
            .y = fb->pos.y,
            .width = fb->width,
            .height = fb->height,
        },
        (Vector2){
            .x = fb->width/2,
            .y = fb->height/2
        },
        fb->attackAngle,
        ORANGE
    );

    // collision probes
    for ( int i = 0; i < 12; i++ ) {

        PolarCoord p = fb->collisionProbes[i];

        DrawCircle( 
            fb->pos.x + cos( toRadians( fb->attackAngle + p.angle ) ) * p.distance,
            fb->pos.y + sin( toRadians( fb->attackAngle + p.angle ) ) * p.distance,
            4, BLUE );

    }
}

void drawPipe( const PipeCouple *pc ) {

    float wideNarrowDiff = ( pc->wideWidth - pc->narrowWidth ) / 2;
    float x = pc->x + wideNarrowDiff;
    float y = 0;

    for ( int i = 0; i < pc->narrowSlicesUp; i++ ) {

        if ( i != 0 ) {
            y += pc->sliceHeight;
        }

        DrawRectangle( 
            x, y, 
            pc->narrowWidth, pc->sliceHeight, 
            pc->color );

    }

    x -= wideNarrowDiff;
    y += pc->sliceHeight;

    DrawRectangle( 
        x, y, 
        pc->wideWidth, pc->sliceHeight, 
        pc->color );

    y += pc->verticalGap + pc->sliceHeight;

    DrawRectangle( 
        x, y, 
        pc->wideWidth, pc->sliceHeight, 
        pc->color );
    
    x += wideNarrowDiff;

    for ( int i = 0; i < pc->narrowTotalSlices - pc->narrowSlicesUp; i++ ) {

        y += pc->sliceHeight;

        DrawRectangle( 
            x, y, 
            pc->narrowWidth, pc->sliceHeight, 
            pc->color );

    }

}

void drawPipes( const PipeCouple *pcs ) {

    for ( int i = 0; i < PIPE_COUPLE_QUANTITY; i++ ) {
        drawPipe( &pcs[i] );
    }

}

void resetGame( GameWorld *gw ) {

    FlappyBird *fb = gw->fb;
    PipeCouple *pcs = gw->pcs;
    Ground *ground = gw->ground;

    fb->pos.x = FLAPPY_BIRD_X_START;
    fb->pos.y = FLAPPY_BIRD_Y_START;
    fb->vel.y = FLAPPY_BIRD_BASE_Y_VELOCITY;
    fb->attackAngle = FLAPPY_BIRD_START_ATTACK_ANGLE;
    fb->state = FLAPPY_BIRD_STATE_IDLE;

}

void createGameWorld( void ) {

    float w2 = FLAPPY_BIRD_WIDTH / 2;
    float h2 = FLAPPY_BIRD_HEIGHT / 2;
    float w4 = FLAPPY_BIRD_WIDTH / 4;
    float h4 = FLAPPY_BIRD_HEIGHT / 4;

    fb = (FlappyBird) {
        .pos = {
            .x = FLAPPY_BIRD_X_START,
            .y = FLAPPY_BIRD_Y_START
        },
        .vel = {
            .x = 0,
            .y = FLAPPY_BIRD_BASE_Y_VELOCITY
        },
        .width = FLAPPY_BIRD_WIDTH,
        .height = FLAPPY_BIRD_HEIGHT,
        .jumpSpeed = FLAPPY_BIRD_JUMP_SPEED,
        .attackAngle = FLAPPY_BIRD_START_ATTACK_ANGLE,
        .minAttackAngle = FLAPPY_BIRD_MIN_ATTACK_ANGLE,
        .maxAttackAngle = FLAPPY_BIRD_MAX_ATTACK_ANGLE,
        .texture = {0},
        .color = ORANGE,
        .state = FLAPPY_BIRD_STATE_IDLE,
        .collisionProbes = {
            { .angle = 0, .distance = w2 },
            { .angle = 90, .distance = h2 },
            { .angle = 180, .distance = w2 },
            { .angle = 270, .distance = h2 },
            { .angle = 35, .distance = h2 + 3 },
            { .angle = 145, .distance = h2 + 3 },
            { .angle = 215, .distance = h2 + 3 },
            { .angle = 325, .distance = h2 + 3 },
            { .angle = 0, .distance = w4 },
            { .angle = 90, .distance = h4 },
            { .angle = 180, .distance = w4 },
            { .angle = 270, .distance = h4 }
        }
    };

    ground = (Ground){
        .pos = {
            .x = 0,
            .y = GetScreenHeight() - GROUND_HEIGHT,
        },
        .vel = {
            .x = 0,
            .y = 0
        },
        .width = GetScreenWidth(),
        .height = GROUND_HEIGHT,
        .texture = {0},
        .color = BEIGE
    };

    gw = (GameWorld){
        .fb = &fb,
        .pcs = {0},
        .ground = &ground
    };

    for ( int i = 0; i < PIPE_COUPLE_QUANTITY; i++ ) {
        gw.pcs[i] = (PipeCouple){
            .x = PIPE_COUPLE_X_START + ( PIPE_WIDE_WIDTH + PIPE_COUPLE_HORIZONTAL_GAP ) * i,
            .wideWidth = PIPE_WIDE_WIDTH,
            .narrowWidth = PIPE_NARROW_WIDTH,
            .sliceHeight = PIPE_SLICE_HEIGHT,
            .narrowSlicesUp = GetRandomValue( 1, 7 ),
            .narrowTotalSlices = PIPE_COUPLE_NARROW_TOTAL_SLICES,
            .verticalGap = PIPE_COUPLE_VERTICAL_GAP,
            .texture = (Texture2D){0},
            .color = LIME
        };
    }

}

void destroyGameWorld( void ) {

}