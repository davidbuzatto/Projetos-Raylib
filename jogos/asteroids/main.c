/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Mais function and logic for the game. Base template for game
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
#define MAX_BULLETS 200

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Bullet {
    Vector2 pos;
    Vector2 vel;
    float speed;
    int radius;
    Color color;
    bool shouldBeRemoved;
    int lifeSpan;
} Bullet;

typedef struct Ship {
    Vector2 pos;
    Vector2 vel;
    int length;
    int frontLength;
    int backLength;
    Color color;
    float speed;
    float speedIncrement;
    float speedDecrement;
    float maxSpeed;
    float angle;
    float angleIncrement;
    Bullet bullets[MAX_BULLETS];
    int bulletQuantity;
} Ship;

typedef struct GameWorld {
    Ship *ship;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Ship ship;


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

void drawShip( const Ship *ship );
void drawBullet( const Bullet *bullet );
void shootBullet( Ship *ship );

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
    const int screenHeight = 450;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Asteroids" );
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

    Ship *ship = gw->ship;

    ship->pos.x += ship->vel.x;
    ship->pos.y += ship->vel.y;

    ship->pos.x = ship->pos.x > GetScreenWidth() ? 0 : ship->pos.x;
    ship->pos.y = ship->pos.y > GetScreenHeight() ? 0 : ship->pos.y;
    ship->pos.x = ship->pos.x < 0 ? GetScreenWidth() : ship->pos.x;
    ship->pos.y = ship->pos.y < 0 ? GetScreenHeight() : ship->pos.y;

    if ( IsKeyDown( KEY_RIGHT ) || IsKeyDown( KEY_D ) ) {
        ship->angle += ship->angleIncrement;
    }
    
    if ( IsKeyDown( KEY_LEFT ) || IsKeyDown( KEY_A ) ) {
        ship->angle -= ship->angleIncrement;
    }

    if ( IsKeyDown( KEY_UP ) || IsKeyDown( KEY_W ) ) {
        ship->speed += ship->speedIncrement;
        if ( ship->speed > ship->maxSpeed ) {
            ship->speed = ship->maxSpeed;
        }
        ship->vel.x = ship->speed * cos( toRadians( ship->angle ) );
        ship->vel.y = ship->speed * sin( toRadians( ship->angle ) );
    } else {
        ship->speed -= ship->speedDecrement;
        if ( ship->speed < 0 ) {
            ship->speed = 0;
        }
    }

    if ( IsKeyDown( KEY_SPACE ) || IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        shootBullet( ship );
    }

    for ( int i = 0; i < ship->bulletQuantity; i++ ) {
        
        Bullet *b = &ship->bullets[i];
        b->pos.x += b->vel.x;
        b->pos.y += b->vel.y;

        b->pos.x = ( (int) b->pos.x ) % GetScreenWidth();
        b->pos.y = ( (int) b->pos.y ) % GetScreenHeight();
        b->pos.x = b->pos.x < 0 ? GetScreenWidth() : b->pos.x;
        b->pos.y = b->pos.y < 0 ? GetScreenHeight() : b->pos.y;

        b->lifeSpan--;
        if ( b->lifeSpan == 0 ) {
            b->shouldBeRemoved = true;
        }

    }

    // bullet cleanup
    int removedQuantity = 0;
    for ( int i = 0; i < ship->bulletQuantity; i++ ) {
        Bullet *b = &ship->bullets[i];
        if ( b->shouldBeRemoved ) {
            for ( int j = i+1; j < ship->bulletQuantity; j++ ) {
                ship->bullets[j-1] = ship->bullets[j];
            }
            removedQuantity++;
        }
    }
    ship->bulletQuantity -= removedQuantity;

    /*ship->vel.x = ship->speed * cos( toRadians( ship->angle ) );
    ship->vel.y = ship->speed * sin( toRadians( ship->angle ) );*/

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    drawShip( gw->ship );

    EndDrawing();

}

void drawShip( const Ship *ship ) {
    
    Vector2 points[] = {{ 
            .x = ship->pos.x + ship->frontLength * cos( toRadians( ship->angle ) ), 
            .y = ship->pos.y + ship->frontLength * sin( toRadians( ship->angle ) )
        }, { 
            .x = ship->pos.x + ship->backLength * cos( toRadians( ship->angle + 135 ) ), 
            .y = ship->pos.y + ship->backLength * sin( toRadians( ship->angle + 135 ) ), 
        }, { 
            .x = ship->pos.x, 
            .y = ship->pos.y
        }, { 
            .x = ship->pos.x + ship->backLength * cos( toRadians( ship->angle + 225 ) ), 
            .y = ship->pos.y + ship->backLength * sin( toRadians( ship->angle + 225 ) ),
        }, { 
            .x = ship->pos.x + ship->frontLength * cos( toRadians( ship->angle ) ), 
            .y = ship->pos.y + ship->frontLength * sin( toRadians( ship->angle ) )
        }
    };

    DrawSplineLinear( points, 5, 1, ship->color );

    for ( int i = 0; i < ship->bulletQuantity; i++ ) {
        if ( !ship->bullets[i].shouldBeRemoved ) {
            drawBullet( &ship->bullets[i] );
        }
    }

}

void drawBullet( const Bullet *bullet ) {
    DrawCircle( bullet->pos.x, bullet->pos.y, bullet->radius, bullet->color );
}

void shootBullet( Ship *ship ) {

    if ( ship->bulletQuantity < MAX_BULLETS ) {

        Bullet newBullet = {
            .pos = {
                .x = ship->pos.x + ship->frontLength * cos( toRadians( ship->angle ) ),
                .y = ship->pos.y + ship->frontLength * sin( toRadians( ship->angle ) )
            },
            .vel = {0},
            .speed = 6,
            .radius = 2,
            .color = { GetRandomValue( 50, 255 ), GetRandomValue( 50, 255 ), GetRandomValue( 50, 255 ), 255 } ,
            .shouldBeRemoved = false,
            .lifeSpan = 200
        };

        newBullet.vel.x = newBullet.speed * cos( toRadians( ship->angle ) );
        newBullet.vel.y = newBullet.speed * sin( toRadians( ship->angle ) );

        ship->bullets[ship->bulletQuantity++] = newBullet;

    }

}

void createGameWorld( void ) {

    ship = (Ship) {
        .pos = {
            .x = GetScreenWidth() / 2,
            .y = GetScreenHeight() / 2
        },
        .vel = {
            .x = 0,
            .y = 0
        },
        .length = 30,
        .frontLength = 0,
        .backLength = 0,
        .color = WHITE,
        .speed = 0,
        .speedIncrement = .1,
        .speedDecrement = .01,
        .maxSpeed = 4,
        .angle = 0,
        .angleIncrement = 3,
        .bullets = {0},
        .bulletQuantity = 0
    };
    ship.backLength = ship.length / 3;
    ship.frontLength = ship.backLength * 2;

    gw = (GameWorld) {
        .ship = &ship
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