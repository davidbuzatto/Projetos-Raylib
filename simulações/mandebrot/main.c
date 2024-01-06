/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Mandelbrot fractal using Raylib (https://www.raylib.com/).
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
#include <raymath.h>

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include <utils.h>

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct ColorBar {
    int x;
    int y;
    int width;
    int height;
    int increment;
    int hueStart;
    int hueEnd;
} ColorBar;

/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
ColorBar colorBar;


int maxIterations = 50;
float zoom = 1.0;
bool colored = false;
int hueStart = 180;
int hueEnd = 282;

/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( void );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( void );
void drawColorBar( ColorBar *colorBar );

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 800;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Conjunto Mandelbrot" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    colorBar = (ColorBar) {
        .x = 20,
        .y = 20,
        .width = GetScreenWidth() - 40,
        .height = 20,
        .increment = 15,
        .hueStart = 0,
        .hueEnd = 360
    };

    while ( !WindowShouldClose() ) {
        inputAndUpdate();
        draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( void ) {

    if ( IsKeyDown( KEY_LEFT_CONTROL ) ) {
        if ( IsKeyDown( KEY_UP ) ) {
            colorBar.hueStart += 5;
            if ( colorBar.hueStart > colorBar.hueEnd ) {
                colorBar.hueStart = colorBar.hueEnd;
            }
        } else if ( IsKeyDown( KEY_DOWN ) ) {
            colorBar.hueStart -= 5;
            if ( colorBar.hueStart < 0 ) {
                colorBar.hueStart = 0;
            }
        }
    } else if ( IsKeyDown( KEY_LEFT_ALT ) ) {
        if ( IsKeyDown( KEY_UP ) ) {
            colorBar.hueEnd += 5;
            if ( colorBar.hueEnd > 360 ) {
                colorBar.hueEnd = 360;
            }
        } else if ( IsKeyDown( KEY_DOWN ) ) {
            colorBar.hueEnd -= 5;
            if ( colorBar.hueEnd < colorBar.hueStart ) {
                colorBar.hueEnd = colorBar.hueStart;
            }
        }
    } else {
        if ( IsKeyPressed( KEY_UP ) ) {
            maxIterations += 10;
        } else if ( IsKeyPressed( KEY_DOWN ) ) {
            maxIterations -= 10;
            if ( maxIterations < 0 ) {
                maxIterations = 0;
            }
        }
    }

    if ( IsKeyPressed( KEY_RIGHT ) ) {
        zoom -= 0.1;
        if ( zoom <= 0 ) {
            zoom = 0.1;
        }
    } else if ( IsKeyPressed( KEY_LEFT ) ) {
        zoom += 0.1;
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        colored = !colored;
    }    

}

void draw( void ) {

    BeginDrawing();
    ClearBackground( WHITE );

    // based on https://en.wikipedia.org/wiki/Mandelbrot_set
    
    Color color = { 0, 0, 0, 255 };

    // escalonar x entre -2.00 e 0.47
    // escalonar y entre -1.12 e 1.12
    float minX = -2.00;
    float maxX = 0.47;
    float minY = -1.12;
    float maxY = 1.12;

    float dx = fabs( maxX - minX ) * (1-zoom) / 2;
    float dy = fabs( maxY - minY ) * (1-zoom) / 2;

    minX += dx;
    maxX -= dx;
    minY += dy;
    maxY -= dy;

    for ( int i = 0; i < GetScreenHeight(); i++ ) {
        for ( int j = 0; j < GetScreenWidth(); j++ ) {

            int px = j;
            int py = i;
            float x = 0.0;
            float y = 0.0;

            float x0 = Lerp( minX, maxX, ( (float) j / (GetScreenWidth()-1) ) );
            float y0 = Lerp( minY, maxY, ( (float) i / (GetScreenHeight()-1) ) );
            
            int k;
            for ( k = 0; k < maxIterations && x*x + y*y <= 2*2; k++ ) {
                float xt = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xt;
            }

            if ( colored ) {
                //color = ColorFromHSV( 360 * ( k / (float) maxIterations ), 1, 0.7 );
                color = ColorFromHSV( colorBar.hueStart + ( colorBar.hueEnd - colorBar.hueStart ) * ( k / (float) maxIterations ), 1, 0.7 );
            } else {
                int c = 255 - 255 * ( k / (float) maxIterations );
                color.r = c;
                color.g = c;
                color.b = c;
            }

            DrawPixel( px, py, color );

        }
    }

    if ( colored ) {
        drawColorBar( &colorBar );
    }

    EndDrawing();

}

void drawColorBar( ColorBar *colorBar ) {

    int quant = 360 / colorBar->increment;
    int t_width = colorBar->width / quant;

    for ( int i = 0; i < quant; i++ ) {
        DrawRectangleGradientH( 
            colorBar->x + i * t_width, 
            colorBar->y, 
            t_width, 
            colorBar->height, 
            ColorFromHSV( i * colorBar->increment, 1, 1 ),
            ColorFromHSV( (i+1) * colorBar->increment, 1, 1 ) );
    }

    DrawCircle( 
        Lerp( colorBar->x, colorBar->width, colorBar->hueStart / 360.0 ),
        colorBar->y + colorBar->height, 5, BLACK );

    DrawCircle( 
        Lerp( colorBar->x, colorBar->width, colorBar->hueEnd / 360.0 ),
        colorBar->y + colorBar->height, 5, BLACK );

}
