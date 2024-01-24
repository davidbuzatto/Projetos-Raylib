/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Pithagoras Tree (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>
#include <utils.h>

typedef struct Square {
    Vector2 center;
    float radius;
    float angle;
    Color color;
    Color strokeColor;
} Square;

Square *squares;
int *orderCount;
int maxOrder = 4;
const int MAX_ALLOWED_ORDER = 15;

float percentage = 1;

/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate();

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw();

void drawPythagorasTree( 
    Vector2 center, float radius, float angle, 
    int currentOrder, int maxOrder, Color baseColor );

void createPythagorasTree( 
    Vector2 center, float radius, float angle, 
    int currentOrder, int maxOrder, Color baseColor, Color toColor );

void createSquares( void );

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

    const int screenWidth = 1200;
    const int screenHeight = screenWidth / 6 * 4;

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Árvore de Pitágoras" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate();
        draw();
    }
    unloadResources();
    destroyGameWorld();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate() {

    if ( IsKeyPressed( KEY_UP ) ) {
        maxOrder++;
        if ( maxOrder > MAX_ALLOWED_ORDER ) {
            maxOrder = MAX_ALLOWED_ORDER;
        }
        createSquares();
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        maxOrder--;
        if ( maxOrder < 1 ) {
            maxOrder = 1;
        }
        createSquares();
    } else if ( IsKeyPressed( KEY_RIGHT ) ) {
        percentage += 0.1;
        if ( percentage > 2 ) {
            percentage = 2;
        }
        createSquares();
    } else if ( IsKeyPressed( KEY_LEFT ) ) {
        percentage -= 0.1;
        if ( percentage < 0.1 ) {
            percentage = 0.1;
        }
        createSquares();
    }

}

void draw() {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < maxOrder; i++ ) {
        for ( int j = 0; j < orderCount[i]; j++ ) {
            Square *s = &squares[i*(1<<maxOrder)+j];
            DrawPoly( s->center, 4, s->radius, s->angle, s->color );
            DrawPolyLinesEx( s->center, 4, s->radius, s->angle, 2, s->strokeColor );
        }
    }

    DrawText( TextFormat( "Ordem: %d", maxOrder-1 ), 20, 20, 20, BLACK );

    EndDrawing();

}



void drawPythagorasTree( Vector2 center, float radius, float angle, 
                         int currentOrder, int maxOrder, Color baseColor ) {

    if ( currentOrder < maxOrder ) {

        float hyp = sqrt( 2 * radius * radius );
        float nRadius = hyp / 2;
        
        Vector3 hsv = ColorToHSV( baseColor );
        Color currentColor = ColorFromHSV( hsv.x, hsv.y, Lerp( hsv.z, 0.2, currentOrder / (float) maxOrder ) );
        DrawPoly( center, 4, radius, angle, currentColor );

        /*Vector2 v1 = {
            .x = center.x + radius * cos( toRadians( angle + 90 ) ),
            .y = center.y + radius * sin( toRadians( angle + 90 ) )
        };
        Vector2 v2 = {
            .x = center.x + radius * cos( toRadians( angle ) ),
            .y = center.y + radius * sin( toRadians( angle ) )
        };*/
        Vector2 v3 = {
            .x = center.x + radius * cos( toRadians( angle - 90 ) ),
            .y = center.y + radius * sin( toRadians( angle - 90 ) )
        };
        Vector2 v4 = {
            .x = center.x + radius * cos( toRadians( angle - 180 ) ),
            .y = center.y + radius * sin( toRadians( angle - 180 ) )
        };

        /*DrawCircleV( v1, 5, RED );
        DrawCircleV( v2, 5, GREEN );
        DrawCircleV( v3, 5, ORANGE );
        DrawCircleV( v4, 5, PURPLE );*/

        Vector2 c1 = {
            .x = v4.x + nRadius * cos( toRadians( angle - 135 ) ),
            .y = v4.y + nRadius * sin( toRadians( angle - 135 ) )
        };
        Vector2 c2 = {
            .x = v3.x + nRadius * cos( toRadians( angle - 135 ) ),
            .y = v3.y + nRadius * sin( toRadians( angle - 135 ) )
        };

        /*DrawCircleV( c1, 5, BLACK );
        DrawCircleV( c2, 5, BLACK );*/

        drawPythagorasTree( c1, nRadius, angle - 45, currentOrder+1, maxOrder, baseColor );
        drawPythagorasTree( c2, nRadius, angle + 45, currentOrder+1, maxOrder, baseColor );

    }

}

void createPythagorasTree( Vector2 center, float radius, float angle, 
                           int currentOrder, int maxOrder, Color baseColor, Color toColor ) {

    if ( currentOrder < maxOrder ) {

        float hyp = sqrt( 2 * radius * radius );
        float nRadius = hyp / 2;

        Color currentColor = {
            .r = Lerp( baseColor.r, toColor.r, currentOrder / (float) maxOrder ),
            .g = Lerp( baseColor.g, toColor.g, currentOrder / (float) maxOrder ),
            .b = Lerp( baseColor.b, toColor.b, currentOrder / (float) maxOrder ),
            .a = 255
        };
        squares[currentOrder*(1<<maxOrder) + orderCount[currentOrder]] = (Square){
            .center = center,
            .radius = radius,
            .angle = angle,
            .color = currentColor,
            .strokeColor = toColor
        };
        orderCount[currentOrder]++;

        Vector2 v3 = {
            .x = center.x + radius * cos( toRadians( angle - 90 ) ),
            .y = center.y + radius * sin( toRadians( angle - 90 ) )
        };
        Vector2 v4 = {
            .x = center.x + radius * cos( toRadians( angle - 180 ) ),
            .y = center.y + radius * sin( toRadians( angle - 180 ) )
        };

        Vector2 c1 = {
            .x = v4.x + nRadius * cos( toRadians( angle - 135 ) ),
            .y = v4.y + nRadius * sin( toRadians( angle - 135 ) )
        };
        Vector2 c2 = {
            .x = v3.x + nRadius * cos( toRadians( angle - 135 ) ),
            .y = v3.y + nRadius * sin( toRadians( angle - 135 ) )
        };

        createPythagorasTree( c1, nRadius, angle - 45, currentOrder+1, maxOrder, baseColor, toColor );
        createPythagorasTree( c2, nRadius, angle + 45, currentOrder+1, maxOrder, baseColor, toColor );

    }

}

void createSquares( void ) {

    if ( squares != NULL ) {
        free( squares );
        free( orderCount );
    }

    int q = (1 << maxOrder) + 1;
    squares = (Square*) malloc( ( q * q ) * sizeof( Square* ) );
    orderCount = (int*) malloc( (maxOrder+1) * sizeof( int ) );

    for ( int i = 0; i <= maxOrder; i++ ) {
        orderCount[i] = 0;
    }

    float squareLength2 = GetScreenWidth() / 12;
    float radius = sqrt( 2 * squareLength2 * squareLength2 );

    createPythagorasTree( 
        (Vector2){ 
            GetScreenWidth() / 2,
            GetScreenHeight() - squareLength2
        },
        radius,
        45, 0, maxOrder, GetColor( 0xffff66ff ), GetColor( 0x148966ff ) );

}

void createGameWorld( void ) {
    printf( "creating game world...\n" );
    squares = NULL;
    orderCount = NULL;
    createSquares();
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