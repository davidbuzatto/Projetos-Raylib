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
#define MAX_BALLS 100

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const Color CUBE_COLOR = { 0, 0, 0, 100 };
const float GRAVITY = -.01;
const float FRICTION = 0.99;
const float ELASTICITY = 0.9;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Ball {
    Vector3 pos;
    Vector3 vel;
    float radius;
    int rings;
    int slices;
    Color color;
} Ball;

typedef struct Cube {
    Vector3 pos;
    int width;
    int height;
    int length;
    Color color;
} Cube;

typedef struct GameWorld {
    Ball balls[MAX_BALLS];
    int ballQuantity;
    Cube *cube;
    Camera3D *camera;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
Cube cube;
GameWorld gw;
Camera3D camera;


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

void drawBall( const Ball *ball );
void drawBallInfo( const Ball *ball );
void drawCube( const Cube *cube );
Ball createBall( void );

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
    InitWindow( screenWidth, screenHeight, "Bolinha 3D" );
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

int xPress;
int yPress;

void inputAndUpdate( GameWorld *gw ) {

    Cube *cube = gw->cube;
    Camera3D *camera = gw->camera;

    if ( IsKeyDown( KEY_RIGHT ) ) {
        camera->position.x++;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        camera->position.x--;
    } else if ( IsKeyDown( KEY_UP ) ) {
        camera->position.y++;
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        camera->position.y--;
    } else if ( IsKeyDown( KEY_A ) ) {
        camera->position.z++;
    } else if ( IsKeyDown( KEY_S ) ) {
        camera->position.z--;
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        xPress = GetMouseX();
        yPress = GetMouseY();
    } else if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        camera->position.x = GetMouseX() - xPress;
        camera->position.y = GetMouseY() - yPress;
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        for ( int i = 0; i < gw->ballQuantity; i++ ) {
            Ball *ball = &gw->balls[i];
            ball->vel.x = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 2, 10 ) / 10.f;
            ball->vel.y = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 2, 10 ) / 10.f;
            ball->vel.z = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 2, 10 ) / 10.f;
        }
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) ) {
        if ( gw->ballQuantity < MAX_BALLS ) {
            gw->balls[gw->ballQuantity++] = createBall();
        }
    }

    for ( int i = 0; i < gw->ballQuantity; i++ ) {

        Ball *ball = &gw->balls[i];
        ball->pos.x += ball->vel.x;
        ball->pos.y += ball->vel.y;
        ball->pos.z += ball->vel.z;

        if ( ball->pos.x + ball->radius > cube->pos.x + cube->width/2 ) {
            ball->pos.x = cube->pos.x + cube->width/2 - ball->radius;
            ball->vel.x = -ball->vel.x * ELASTICITY;
        } else if ( ball->pos.x - ball->radius < cube->pos.x - cube->width/2 ) {
            ball->pos.x = cube->pos.x - cube->width/2 + ball->radius; 
            ball->vel.x = -ball->vel.x * ELASTICITY;
        } else if ( ball->pos.y + ball->radius > cube->pos.y + cube->height/2 ) {
            ball->pos.y = cube->pos.y + cube->height/2 - ball->radius;
            ball->vel.y = -ball->vel.y * ELASTICITY;
        } else if ( ball->pos.y - ball->radius < cube->pos.y - cube->height/2 ) {
            ball->pos.y = cube->pos.y - cube->height/2 + ball->radius;
            ball->vel.y = -ball->vel.y * ELASTICITY;
        } else if ( ball->pos.z + ball->radius > cube->pos.z + cube->length/2 ) {
            ball->pos.z = cube->pos.z + cube->length/2 - ball->radius;
            ball->vel.z = -ball->vel.z * ELASTICITY;
        } else if ( ball->pos.z - ball->radius < cube->pos.z - cube->length/2 ) {
            ball->pos.z = cube->pos.z - cube->length/2 + ball->radius;
            ball->vel.z = -ball->vel.z * ELASTICITY;
        }

        ball->vel.x = ball->vel.x * FRICTION;
        ball->vel.y = ball->vel.y * FRICTION + GRAVITY;
        ball->vel.z = ball->vel.z * FRICTION;

    }

    camera->target.x = cube->pos.x;
    camera->target.y = cube->pos.y;
    camera->target.z = cube->pos.z;

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );
    BeginMode3D( *(gw->camera) );

    DrawGrid( 100, 1.0f );
    DrawPlane( (Vector3){ 0, 0, 0 }, (Vector2){ gw->cube->width, gw->cube->length }, DARKBLUE );
    for ( int i = 0; i < gw->ballQuantity; i++ ) {
        drawBall( &gw->balls[i] );
    }
    drawCube( gw->cube );

    EndMode3D();
    EndDrawing();

}

void drawBall( const Ball *ball ) {
    DrawSphereEx( ball->pos, ball->radius, ball->rings, ball->slices, ball->color );
}

void drawBallInfo( const Ball *ball ) {
    DrawText( 
        TextFormat( "x: %.2f\ny: %.2f\nz: %.2f", ball->pos.x, ball->pos.y, ball->pos.z ), 
        20, 20, 20, BLACK );
}

void drawCube( const Cube *cube ) {
    DrawCubeWires( cube->pos, cube->width, cube->height, cube->length, cube->color );
    DrawCube( cube->pos, cube->width, cube->height, cube->length, Fade( cube->color, 0.2f ) );
}

Ball createBall( void ) {
    return (Ball) {
        .pos = {
            .x = 0,
            .y = 10,
            .z = 0
        },
        .vel = {
            .x = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 5, 10 ) / 10.f,
            .y = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 5, 10 ) / 10.f,
            .z = ( GetRandomValue( 0, 1 ) == 0 ? -1 : 1 ) * GetRandomValue( 5, 10 ) / 10.f
        },
        .radius = GetRandomValue( 1, 10 ) / 10.f,
        .rings = 20,
        .slices = 20,
        .color = (Color) {
            GetRandomValue( 100, 255 ),
            GetRandomValue( 100, 255 ),
            GetRandomValue( 100, 255 ),
            255
        }
    };
}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    cube = (Cube) {
        .pos = {
            .x = 0,
            .y = 10,
            .z = 0
        },
        .width = 40,
        .height = 20,
        .length = 20,
        .color = DARKBLUE
    };

    camera = (Camera3D) {
        // Camera position
        .position = (Vector3){ 0.0f, 5.0f, 20.0f },
        // Camera looking at point
        .target = (Vector3){ cube.pos.x, cube.pos.y, cube.pos.z },
        // Camera up vector (rotation towards target)
        .up = (Vector3){ 0.0f, 1.0f, 0.0f },
        // Camera field-of-view Y
        .fovy = 100.0f,
        // Camera mode type
        .projection = CAMERA_PERSPECTIVE
    };

    gw = (GameWorld) {
        .balls = {0},
        .ballQuantity = 0,
        .cube = &cube,
        .camera = &camera
    };

    gw.balls[gw.ballQuantity++] = (Ball) {
        .pos = {
            .x = 0,
            .y = cube.height/2,
            .z = 0
        },
        .vel = {
            .x = .5,
            .y = -.5,
            .z = .5
        },
        .radius = 1,
        .rings = 20,
        .slices = 20,
        .color = BLUE
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