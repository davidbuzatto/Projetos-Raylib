/*******************************************************************************
 * Puzzle Bobble using Raylib (https://www.raylib.com/)
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

const int BALL_RADIUS = 20;
const int BALL_SPEED = 10;

typedef struct Ball {
    Vector2 pos;
    Vector2 vel;
    int speed;
    int radius;
    double angle;
    Color color;
} Ball;

typedef struct Cannon {
    Vector2 pos;
    int aimSightLength;
    double angle;
    Color color;
    Ball *currentBall;
    double minAngle;
    double maxAngle;
} Cannon;

typedef struct {
    Cannon *cannon;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void drawBall( Ball *ball );
void drawCannon( Cannon *cannon );


double toRadians( double degrees );
double toDegrees( double radians );

int main( void ) {

    const int screenWidth = 400;
    const int screenHeight = 700;

    Ball ball = {
        .pos = {
            .x = screenWidth / 2,
            .y = screenHeight - 100
        },
        .vel = {
            .x = 0,
            .y = 0
        },
        .speed = BALL_SPEED,
        .radius = BALL_RADIUS,
        .angle = 0,
        .color = BLUE
    };

    Cannon cannon = {
        .pos = {
            .x = screenWidth / 2,
            .y = screenHeight - 100
        },
        .aimSightLength = screenHeight,
        .angle = -90,
        .minAngle = -170,
        .maxAngle = -10,
        .color = BLACK,
        .currentBall = &ball
    };

    GameWorld gw = {
        .cannon = &cannon
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Puzzle Bobble" );
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

    Cannon *cannon = gw->cannon;
    Ball *ball = cannon->currentBall;

    if ( IsKeyDown( KEY_RIGHT ) ) {
        if ( cannon->angle < cannon->maxAngle ) {
            cannon->angle++;
        } else {
            cannon->angle = cannon->maxAngle;
        }
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        if ( cannon->angle > cannon->minAngle ) {
            cannon->angle--;
        } else {
            cannon->angle = cannon->minAngle;
        }
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        ball->vel.x = ball->speed * cos( toRadians( cannon->angle ) );
        ball->vel.y = ball->speed * sin( toRadians( cannon->angle ) );
        ball->angle = cannon->angle;
    }

    ball->pos.x += ball->vel.x;
    ball->pos.y += ball->vel.y;

    if ( ball->pos.x + ball->radius >= GetScreenWidth() ) {
        ball->pos.x = GetScreenWidth() - ball->radius;
        ball->vel.x = -ball->vel.x;
    } else if ( ball->pos.x - ball->radius <= 0 ) {
        ball->pos.x = ball->radius;
        ball->vel.x = -ball->vel.x;
    } else if ( ball->pos.y - ball->radius <= 0 ) {
        // top container collision
        /*ball->pos.y = ball->radius;
        ball->vel.y = -ball->vel.y;*/
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawBall( gw->cannon->currentBall );
    drawCannon( gw->cannon );

    EndDrawing();

}

void drawBall( Ball *ball ) {

    DrawCircle( ball->pos.x, ball->pos.y, ball->radius, ball->color );

}

void drawCannon( Cannon *cannon ) {

    double ra = toRadians( cannon->angle );
    double rac = toRadians( 180 - cannon->angle );
    double w = GetScreenWidth() / 2;
    double ca = cannon->aimSightLength * cos( ra );
    double co = cannon->aimSightLength * sin( ra );

    double a = tan( ra );
    double x = cannon->pos.x;
    double y = cannon->pos.y - fabs( a * GetScreenWidth() / 2 );

    if ( a < 0 ) {
        x += w;
    } else {
        x += -w;
    }

    DrawLine( 
        (int) cannon->pos.x, 
        (int) cannon->pos.y, 
        (int) x, 
        (int) y,
        cannon->color );

    DrawLine( 
        (int) cannon->pos.x, 
        (int) cannon->pos.y, 
        (int) x - BALL_RADIUS, 
        (int) y,
        RED );

    DrawLine( 
        (int) x, 
        (int ) y, 
        (int) (x + cannon->aimSightLength * cos( rac ) ),
        (int) (y + cannon->aimSightLength * sin( rac ) ), 
        cannon->color );

}


double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}