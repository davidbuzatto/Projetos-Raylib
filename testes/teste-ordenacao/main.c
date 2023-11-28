/*******************************************************************************
 * Teste ordenação.
 *
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "raylib.h"

 // macros


 // enums, structs, unions and custom types
typedef struct {
    int dummy;
} GameWorld;

// global variables
int *array;
int ig;
int jg;
int size;
bool stop;
bool swapped;

// function prototypes
void init( void );
void input( GameWorld* gw );
void update( GameWorld* gw );
void draw( GameWorld* gw );

int main( void ) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    GameWorld gw = {
        .dummy = 0
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT ); // turn antialiasing on (if possible)
    InitWindow( screenWidth, screenHeight, "Window Title" );
    //InitAudioDevice();
    SetTargetFPS( 40 );

    init();

    while ( !WindowShouldClose() ) {
        input( &gw );
        update( &gw );
        draw( &gw );
    }

    //CloseAudioDevice();
    CloseWindow();
    return 0;

}

void init( void ) {

    size = 20;
    ig = 0;
    jg = size-1;
    stop = false;
    swapped = false;

    array = (int*) malloc( size * sizeof(int) );

    for ( int i = 0; i < size; i++ ) {
        array[i] = GetRandomValue( 1, 20 );
    }


}

void input( GameWorld* gw ) {

    if ( stop ) {
        if ( IsKeyPressed( KEY_ENTER ) ) {
            free( array );
            init();
        }
    }

}

void update( GameWorld* gw ) {
    
    int t;

    if ( ig >= size-1 ) {
        stop = true;
    }

    if ( !stop ) {
        if ( array[jg-1] > array[jg] ) {
            t = array[jg-1];
            array[jg-1] = array[jg];
            array[jg] = t;
            swapped = true;
        }
        jg--;
        if ( jg == ig ) {
            jg = size - 1;
            ig++;
            if ( !swapped ) {
                stop = true;
            }
            swapped = false;
        }
    }

}

void draw( GameWorld* gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    int espaco = 10;
    int base = 500;
    int xIni = 50;
    int larguraUnidade = 20;
    Color c;

    for ( int i = 0; i < size; i++ ) {

        if ( !stop ) {
            if ( i == ig ) {
                c = GREEN;
            } else if ( i == jg - 1 ) {
                c = ORANGE;
            } else if ( i == jg ) {
                c = PURPLE;
            } else {
                c = BLUE;
            }
        } else {
            c = BLUE;
        }

        DrawRectangle( 
            xIni + i * larguraUnidade, 
            base - array[i] * larguraUnidade,
            larguraUnidade - 5,
            array[i] * larguraUnidade,
            c );
    }

    EndDrawing();

}