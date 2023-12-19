/*******************************************************************************
 * Angry Birds using C and using Raylib (https://www.raylib.com/).
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

#define MAX_OBSTACLES 10

const int LAUNCH_MAX_DISTANCE = 100;
const float GRAVITY = 7;

typedef struct Ball {
    Vector2 pos;
    Vector2 vel;
    int radius;
    Color fillColor;
    Color strokeColor;
} Ball;

typedef struct Obstacle {
    Rectangle rect;
    Color fillColor;
    Color strokeColor;
} Obstacle;

typedef struct GameWorld {
    Ball *ball;
    Obstacle obstacles[MAX_OBSTACLES];
} GameWorld;

int xPress;
int yPress;
int xOffset;
int yOffset;

float xLaunch;
float yLaunch;
bool launched;
float launchAngle;
float pi;

int obstacleQuantity;

GameWorld gw;
Ball ball;

Ball *selectedBall;

void createGameWorld( void );
void destroyGameWorld( void );

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void drawBall( Ball *ball );
void drawObstacles( Obstacle *obstacles );
bool coordVsBallIntercept( int x, int y, Ball *ball );

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Angry Birds - Prof. Dr. David Buzatto" );
    SetTargetFPS( 60 );    
    createGameWorld();

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    destroyGameWorld();
    CloseWindow();
    return 0;

}

void createGameWorld( void ) {

    xPress = 0;
    yPress = 0;
    xOffset = 0;
    yOffset = 0;

    xLaunch = 200;
    yLaunch = 200;
    launched = false;
    launchAngle = 0;
    pi = acos(-1);
    selectedBall = NULL;

    ball = (Ball){
        .pos = { 
            .x = xLaunch, 
            .y = yLaunch
        },
        .vel = {
            .x = 50,
            .y = -50
        },
        .radius = 30,
        .fillColor = BLUE,
        .strokeColor = DARKBLUE,
    };

    gw = (GameWorld){
        .ball = &ball
    };

    obstacleQuantity = 0;

    for ( int i = 0; i < 3; i++ ) {
        gw.obstacles[obstacleQuantity++] = (Obstacle){
            .rect = {
                .x = 550,
                .y = 80 + 130 * i,
                .width = 30,
                .height = 100
            },
            .fillColor = GREEN,
            .strokeColor = DARKGREEN
        };
        gw.obstacles[obstacleQuantity++] = (Obstacle){
            .rect = {
                .x = 700,
                .y = 80 + 130 * i,
                .width = 30,
                .height = 100
            },
            .fillColor = YELLOW,
            .strokeColor = GOLD
        };
        gw.obstacles[obstacleQuantity++] = (Obstacle){
            .rect = {
                .x = 550,
                .y = 50 + 130 * i,
                .width = 180,
                .height = 30
            },
            .fillColor = RED,
            .strokeColor = MAROON
        };
    }

}

void destroyGameWorld( void ) {

}

void inputAndUpdate( GameWorld *gw ) {

    Ball *ball = gw->ball;

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        if ( coordVsBallIntercept( GetMouseX(), GetMouseY(), ball ) ) {
            selectedBall = ball;
            xOffset = GetMouseX() - selectedBall->pos.x;
            yOffset = GetMouseY() - selectedBall->pos.y;
        }
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {

        if ( selectedBall != NULL ) {

            selectedBall->pos.x = GetMouseX() - xOffset;
            selectedBall->pos.y = GetMouseY() - yOffset;

            int c1 = xLaunch - selectedBall->pos.x;
            int c2 = yLaunch - selectedBall->pos.y;
            int distance = sqrt( c1 * c1 + c2 * c2 );
            double relAngle = atan2( c2, c1 ) + pi;
            launchAngle = pi - relAngle;
            /*double relAngle = toDegrees( atan2( c2, c1 ) ) + 180;
            printf( "%.2f\n", relAngle );*/

            if ( distance >= LAUNCH_MAX_DISTANCE ) {
                selectedBall->pos.x = xLaunch + cos( relAngle ) * LAUNCH_MAX_DISTANCE;
                selectedBall->pos.y = yLaunch + sin( relAngle ) * LAUNCH_MAX_DISTANCE;
            }

        }

    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        if ( selectedBall != NULL ) {
            if ( selectedBall->pos.x != xLaunch && selectedBall->pos.y != yLaunch ) {
                launched = true;
                selectedBall->vel.x =  abs( (int) ( ( selectedBall->pos.x - xLaunch ) * 0.8 ) );
                selectedBall->vel.y = -abs( (int) ( ( selectedBall->pos.y - yLaunch ) * 0.8 ) );
            }
            /*selectedBall->pos.x = xLaunch;
            selectedBall->pos.y = yLaunch;*/
            selectedBall = NULL;
        }
    }

    if ( launched ) {
        ball->pos.x += ball->vel.x;
        ball->pos.y += ball->vel.y;
        ball->vel.x = ball->vel.x * cos( launchAngle );
        ball->vel.y = ball->vel.y * sin( launchAngle );
        ball->vel.y += GRAVITY;

    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) ) {
        selectedBall = NULL;
        ball->pos.x = xLaunch;
        ball->pos.y = yLaunch;
        launched = false;
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawBall( gw->ball );
    drawObstacles( gw->obstacles );

    EndDrawing();

}

void drawBall( Ball *ball ) {

    DrawLine( xLaunch, yLaunch, ball->pos.x, ball->pos.y, BLACK );

    DrawCircle( ball->pos.x, ball->pos.y, ball->radius, ball->strokeColor );
    DrawCircle( ball->pos.x, ball->pos.y, ball->radius-2, ball->fillColor );

    /*char debugData[50];
    sprintf( debugData, "%dx%d", (int) ball->pos.x, (int) ball->pos.y );
    DrawText( debugData, ball->pos.x, ball->pos.y - 10, 10, BLACK );*/

}

void drawObstacles( Obstacle *obstacles ) {

    for ( int i = 0; i < obstacleQuantity; i++ ) {
        DrawRectangleRec( obstacles[i].rect, obstacles[i].fillColor );
        DrawRectangleLinesEx( obstacles[i].rect, 2, obstacles[i].strokeColor );
    }

}

bool coordVsBallIntercept( int x, int y, Ball *ball ) {

    float c1 = x - ball->pos.x;
    float c2 = y - ball->pos.y;

    return c1 * c1 + c2 * c2 <= ball->radius * ball->radius;

}