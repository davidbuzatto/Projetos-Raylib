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

#define MAX_ACTIVE_PIPES 10
#define FLAPPY_BIRD_COLLISION_PROBES_QUANTITY 12

const int GROUND_HEIGHT = 80;
const float GRAVITY = .5;
const Color BACKGROUND_COLOR = { .r = 113, .g = 198, .b = 206, .a = 255 };
const Color COLLISION_RESOLUTION_COLOR = { .r = 0, .g = 121, .b = 241, .a = 150 };
const Color COLLISION_RESOLUTION_COL_COLOR = { .r = 230, .g = 41, .b = 55, .a = 150 };

const float FIRST_PLANE_SPEED = -2;
const float SECOND_PLANE_SPEED = -1;

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
const float FLAPPY_BIRD_JUMP_SPEED = -9;

const int PIPE_COUPLE_X_START = 500;
const int PIPE_WIDE_WIDTH = 90;
const int PIPE_NARROW_WIDTH = 80;
const int PIPE_SLICE_HEIGHT = 40;
const int PIPE_COUPLE_NARROW_TOTAL_SLICES = 8;
const int PIPE_COUPLE_HORIZONTAL_GAP = 180;
const int PIPE_COUPLE_VERTICAL_GAP = 170;
const int PIPE_COUPLE_START_QUANTITY = 5;

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
    Texture *texture;
    FlappyBirdState state;
    PolarCoord collisionProbes[FLAPPY_BIRD_COLLISION_PROBES_QUANTITY];
} FlappyBird;

typedef struct PipeCouple {
    Vector2 pos;
    Vector2 vel;
    int wideWidth;
    int narrowWidth;
    int sliceHeight;
    int narrowSlicesUp;
    int narrowTotalSlices;
    int verticalGap;
    Texture *sliceTexture;
    Texture *mouthUpTexture;
    Texture *mouthDownTexture;
} PipeCouple;

typedef struct Ground {
    Vector2 pos;
    Vector2 vel;
    int width;
    int height;
    Texture *texture;
} Ground;

typedef struct Background {
    Vector2 pos;
    Vector2 vel;
    int width;
    int height;
    Texture *texture;
    Color color;
} Background;

typedef struct GameWorld {
    FlappyBird *fb;
    PipeCouple pcs[MAX_ACTIVE_PIPES];
    Ground *ground;
    Background *background;
} GameWorld;

GameWorld gw;
FlappyBird fb;
Ground ground;
Background background;

int pipeCoupleQuantity = 0;

Texture2D backgroundTextureLoop;
Texture2D flappyBirdTexture;
Texture2D groundTextureLoop;
Texture2D pipeCoupleSliceTexture;
Texture2D pipeCoupleMouthUpTexture;
Texture2D pipeCoupleMouthDownTexture;

// collision resolution
Rectangle pipeUpSlicesRect = {0};
Rectangle pipeMouthUpRect = {0};
Rectangle pipeDownSlicesRect = {0};
Rectangle pipeMouthDownRect = {0};
Vector2 flappyBirdCollisionProbes[FLAPPY_BIRD_COLLISION_PROBES_QUANTITY] = {0};
bool collisionDetected = false;
Rectangle collidedRect = {0};
Vector2 collidedProbe = {0};


void inputAndUpdate( GameWorld *gw );
void draw( const GameWorld *gw );

void drawBackground( const Background *background );
void drawGround( const Ground *ground );
void drawFlappyBird( const FlappyBird *fb );
void drawPipe( const PipeCouple *pc );
void drawPipes( const PipeCouple *pcs );
void drawCollisionResolutionInformation( const GameWorld *gw );

PipeCouple newPipeCouple( int position );
bool flappyBirdVsPipeCoupleIntercept( const FlappyBird *fp, const PipeCouple *pc );

void loadGameResources( void );
void unloadGameResources( void );
void createGameWorld( void );
void destroyGameWorld( void );

int main( void ) {

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( 1200, 650, "Flappy Bird" );
    //InitAudioDevice();
    SetTargetFPS( 60 );    

    loadGameResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    destroyGameWorld();
    unloadGameResources();

    //CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    FlappyBird *fb = gw->fb;
    PipeCouple *pcs = gw->pcs;
    Ground *ground = gw->ground;
    Background *background = gw->background;

    if ( fb->state == FLAPPY_BIRD_STATE_IDLE ) {

        if ( IsKeyPressed( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            fb->state = FLAPPY_BIRD_STATE_ALIVE;
        }

    } else if ( fb->state == FLAPPY_BIRD_STATE_ALIVE ) {

        ground->pos.x += ground->vel.x;
        if ( ground->pos.x < -ground->texture->width ) {
            ground->pos.x = 0;
        }

        background->pos.x += background->vel.x;
        if ( background->pos.x < -background->texture->width ) {
            background->pos.x = 0;
        }

        for ( int i = 0; i < pipeCoupleQuantity; i++ ) {
            
            pcs[i].pos.x += pcs[i].vel.x;

            if ( i == 0 ) {
                if ( pcs[i].pos.x < -pcs[i].wideWidth ) {

                    for ( int j = 1; j < pipeCoupleQuantity; j++ ) {
                        pcs[j-1] = pcs[j];
                    }

                    pipeCoupleQuantity--;
                    PipeCouple *lastP = &pcs[pipeCoupleQuantity];
                    PipeCouple newP = newPipeCouple( pipeCoupleQuantity );
                    newP.pos.x = lastP->pos.x + newP.wideWidth + PIPE_COUPLE_HORIZONTAL_GAP;
                    pcs[pipeCoupleQuantity] = newP;
                    pipeCoupleQuantity++;

                }
            }
        }

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

        if ( flappyBirdVsPipeCoupleIntercept( fb, &pcs[0] ) ||
             flappyBirdVsPipeCoupleIntercept( fb, &pcs[1] ) ) {
            fb->state = FLAPPY_BIRD_STATE_DEAD;
        }
        /*if ( flappyBirdVsPipeCoupleIntercept( fb, &pcs[0] ) ) {
            fb->state = FLAPPY_BIRD_STATE_DEAD;
        }*/

        if ( fb->attackAngle < fb->minAttackAngle ) {
            fb->attackAngle = fb->minAttackAngle;
        } else if ( fb->attackAngle > fb->maxAttackAngle ) {
            fb->attackAngle = fb->maxAttackAngle;
        }

        fb->vel.y += GRAVITY;

    } else if ( fb->state == FLAPPY_BIRD_STATE_DEAD ) {

        if ( IsKeyPressed( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            createGameWorld();
        }

    }

}

void draw( const GameWorld *gw ) {

    BeginDrawing();

    drawBackground( gw->background );
    drawGround( gw->ground );
    drawPipes( gw->pcs );
    drawFlappyBird( gw->fb );

    drawCollisionResolutionInformation( gw );

    EndDrawing();

}

void drawBackground( const Background *background ) {

    ClearBackground( background->color );

    Texture *texture = background->texture;
    int textureWidth = texture->width;
    int quantity = background->width / textureWidth + 3;

    for ( int i = 0; i < quantity; i++ ) {
        DrawTexture( 
            *texture, 
            background->pos.x + textureWidth * i,
            background->pos.y,
            WHITE );
    }

}

void drawGround( const Ground *ground ) {

    Texture *texture = ground->texture;
    int textureWidth = texture->width;
    int quantity = ground->width / textureWidth + 3;

    for ( int i = 0; i < quantity; i++ ) {
        DrawTexture( 
            *texture, 
            ground->pos.x + textureWidth * i,
            ground->pos.y, 
            WHITE );
    }

}

void drawFlappyBird( const FlappyBird *fb ) {
    
    DrawTexturePro(
        *(fb->texture),
        (Rectangle){
            .x = 0,
            .y = 0,
            .width = fb->width,
            .height = fb->height,
        },
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
        WHITE
    );

}

void drawPipe( const PipeCouple *pc ) {

    float wideNarrowDiff = ( pc->wideWidth - pc->narrowWidth ) / 2;
    float x = pc->pos.x + wideNarrowDiff;
    float y = pc->pos.y;

    for ( int i = 0; i < pc->narrowSlicesUp; i++ ) {

        if ( i != 0 ) {
            y += pc->sliceHeight;
        }

        DrawTexture( *(pc->sliceTexture), x, y, WHITE );

    }

    x -= wideNarrowDiff;
    y += pc->sliceHeight;

    DrawTexture( *(pc->mouthUpTexture), x, y, WHITE );

    y += pc->verticalGap + pc->sliceHeight;

    DrawTexture( *(pc->mouthDownTexture), x, y, WHITE );
    
    x += wideNarrowDiff;

    for ( int i = 0; i < pc->narrowTotalSlices - pc->narrowSlicesUp; i++ ) {
        y += pc->sliceHeight;
        DrawTexture( *(pc->sliceTexture), x, y, WHITE );
    }

}

void drawPipes( const PipeCouple *pcs ) {

    for ( int i = 0; i < pipeCoupleQuantity; i++ ) {
        drawPipe( &pcs[i] );
    }

}

void drawCollisionResolutionInformation( const GameWorld *gw ) {

    if ( gw->fb->state != FLAPPY_BIRD_STATE_IDLE ) {

        DrawRectangleRec( pipeUpSlicesRect, COLLISION_RESOLUTION_COLOR );
        DrawRectangleRec( pipeMouthUpRect, COLLISION_RESOLUTION_COLOR );
        DrawRectangleRec( pipeMouthDownRect, COLLISION_RESOLUTION_COLOR );
        DrawRectangleRec( pipeDownSlicesRect, COLLISION_RESOLUTION_COLOR );

        for ( int i = 0; i < FLAPPY_BIRD_COLLISION_PROBES_QUANTITY; i++ ) {
            DrawCircleV( flappyBirdCollisionProbes[i], 4, COLLISION_RESOLUTION_COLOR );
        }

        if ( collisionDetected ) {
            DrawRectangleRec( collidedRect, COLLISION_RESOLUTION_COL_COLOR );
            DrawCircleV( collidedProbe, 4, COLLISION_RESOLUTION_COL_COLOR );
        }

    }

}

bool flappyBirdVsPipeCoupleIntercept( const FlappyBird *fb, const PipeCouple *pc ) {

    pipeUpSlicesRect = (Rectangle) {
        .x = pc->pos.x + (pc->wideWidth - pc->narrowWidth) / 2,
        .y = pc->pos.y,
        .width = pc->narrowWidth,
        .height = pc->sliceHeight * pc->narrowSlicesUp
    };

    pipeMouthUpRect = (Rectangle) {
        .x = pc->pos.x,
        .y = pc->pos.y + pipeUpSlicesRect.height,
        .width = pc->wideWidth,
        .height = pc->sliceHeight
    };

    pipeMouthDownRect = (Rectangle) {
        .x = pc->pos.x,
        .y = pc->pos.y + pipeMouthUpRect.y + pipeMouthUpRect.height + pc->verticalGap,
        .width = pc->wideWidth,
        .height = pc->sliceHeight
    };

    pipeDownSlicesRect = (Rectangle) {
        .x = pc->pos.x + (pc->wideWidth - pc->narrowWidth) / 2,
        .y = pc->pos.y + pipeMouthDownRect.y + pc->sliceHeight,
        .width = pc->narrowWidth,
        .height = pc->sliceHeight * ( pc->narrowTotalSlices - pc->narrowSlicesUp )
    };

    for ( int i = 0; i < FLAPPY_BIRD_COLLISION_PROBES_QUANTITY; i++ ) {

        PolarCoord p = fb->collisionProbes[i];
        flappyBirdCollisionProbes[i].x = fb->pos.x + cos( toRadians( fb->attackAngle + p.angle ) ) * p.distance;
        flappyBirdCollisionProbes[i].y = fb->pos.y + sin( toRadians( fb->attackAngle + p.angle ) ) * p.distance;

    }

    collisionDetected = false;

    for ( int i = 0; i < FLAPPY_BIRD_COLLISION_PROBES_QUANTITY; i++ ) {

        Vector2 v = flappyBirdCollisionProbes[i];
        
        collisionDetected = CheckCollisionPointRec( v, pipeUpSlicesRect );
        if ( collisionDetected ) {
            collidedRect = pipeUpSlicesRect;
            collidedProbe = v;
            break;
        }

        collisionDetected = CheckCollisionPointRec( v, pipeMouthUpRect );
        if ( collisionDetected ) {
            collidedRect = pipeMouthUpRect;
            collidedProbe = v;
            break;
        }

        collisionDetected = CheckCollisionPointRec( v, pipeMouthDownRect );
        if ( collisionDetected ) {
            collidedRect = pipeMouthDownRect;
            collidedProbe = v;
            break;
        }

        collisionDetected = CheckCollisionPointRec( v, pipeDownSlicesRect );
        if ( collisionDetected ) {
            collidedRect = pipeDownSlicesRect;
            collidedProbe = v;
            break;
        }

    }

    return collisionDetected;

}

void loadGameResources( void ) {

    backgroundTextureLoop = LoadTexture( "resources/backgroundTextureLoop.png" );
    flappyBirdTexture = LoadTexture( "resources/flappyBirdTexture.png" );
    groundTextureLoop = LoadTexture( "resources/groundTextureLoop.png" );
    pipeCoupleSliceTexture = LoadTexture( "resources/pipeCoupleSliceTexture.png" );
    pipeCoupleMouthUpTexture = LoadTexture( "resources/pipeCoupleTopUpTexture.png" );
    pipeCoupleMouthDownTexture = LoadTexture( "resources/pipeCoupleTopDownTexture.png" );

}

void unloadGameResources( void ) {

    UnloadTexture( backgroundTextureLoop );
    UnloadTexture( flappyBirdTexture );
    UnloadTexture( groundTextureLoop );
    UnloadTexture( pipeCoupleSliceTexture );
    UnloadTexture( pipeCoupleMouthUpTexture );
    UnloadTexture( pipeCoupleMouthDownTexture );

}

void createGameWorld( void ) {

    pipeCoupleQuantity = 0;
    collisionDetected = false;

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
        .texture = &flappyBirdTexture,
        .state = FLAPPY_BIRD_STATE_IDLE,
        .collisionProbes = {
            { .angle = 8, .distance = w2 },
            { .angle = 90, .distance = h2 },
            { .angle = 177, .distance = w2 },
            { .angle = 270, .distance = h2 },
            { .angle = 35, .distance = h2 },
            { .angle = 145, .distance = h2 + 3 },
            { .angle = 215, .distance = h2 + 3 },
            { .angle = 325, .distance = h2 + 3 },
            { .angle = 0, .distance = w4 },
            { .angle = 90, .distance = h4 },
            { .angle = 180, .distance = w4 },
            { .angle = 270, .distance = h4 }
        }
    };

    ground = (Ground) {
        .pos = {
            .x = 0,
            .y = GetScreenHeight() - GROUND_HEIGHT,
        },
        .vel = {
            .x = FIRST_PLANE_SPEED,
            .y = 0
        },
        .width = GetScreenWidth(),
        .height = GROUND_HEIGHT,
        .texture = &groundTextureLoop
    };

    background = (Background) {
        .pos = {
            .x = 0,
            .y = GetScreenHeight() - GROUND_HEIGHT - backgroundTextureLoop.height,
        },
        .vel = {
            .x = SECOND_PLANE_SPEED,
            .y = 0
        },
        .width = GetScreenWidth(),
        .height = backgroundTextureLoop.height,
        .texture = &backgroundTextureLoop,
        .color = BACKGROUND_COLOR
    };

    gw = (GameWorld){
        .fb = &fb,
        .pcs = {0},
        .ground = &ground,
        .background = &background
    };

    for ( int i = 0; i < PIPE_COUPLE_START_QUANTITY; i++ ) {
        gw.pcs[pipeCoupleQuantity++] = newPipeCouple( i );
    }

}

void destroyGameWorld( void ) {
    printf( "destroying game world..." );
}

PipeCouple newPipeCouple( int position ) {

    return (PipeCouple){
        .pos = {
            .x = PIPE_COUPLE_X_START + ( PIPE_WIDE_WIDTH + PIPE_COUPLE_HORIZONTAL_GAP ) * position,
            .y = 0
        },
        .vel = {
            .x = FIRST_PLANE_SPEED,
            .y = 0
        },
        .wideWidth = PIPE_WIDE_WIDTH,
        .narrowWidth = PIPE_NARROW_WIDTH,
        .sliceHeight = PIPE_SLICE_HEIGHT,
        .narrowSlicesUp = GetRandomValue( 1, 7 ),
        .narrowTotalSlices = PIPE_COUPLE_NARROW_TOTAL_SLICES,
        .verticalGap = PIPE_COUPLE_VERTICAL_GAP,
        .sliceTexture = &pipeCoupleSliceTexture,
        .mouthUpTexture = &pipeCoupleMouthUpTexture,
        .mouthDownTexture = &pipeCoupleMouthDownTexture
    };

}