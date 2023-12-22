/*******************************************************************************
 * Testes com splines
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

#define MAX_POINTS 500

Vector2 points[MAX_POINTS];
int pointQuantity;

typedef struct GameWorld {
    int dummy;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

int main( void ) {

    const int screenWidth = 400;
    const int screenHeight = 400;
    pointQuantity = 0;

    GameWorld gw = {
        .dummy = 0
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Splines" );
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && 
         pointQuantity < MAX_POINTS ) {
        points[pointQuantity++] = (Vector2){
            .x = GetMouseX(),
            .y = GetMouseY()
        };
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    if ( pointQuantity >= 4 ) {
        //DrawSplineLinear( points, pointQuantity, 2, BLACK );
        //DrawSplineBasis( points, pointQuantity, 2, RED );
        //DrawSplineCatmullRom( points, pointQuantity, 2, GREEN );
        //DrawSplineBezierQuadratic( points, pointQuantity, 2, BLUE );
        DrawSplineBezierCubic( points, pointQuantity, 2, PURPLE );
    }

    EndDrawing();

}