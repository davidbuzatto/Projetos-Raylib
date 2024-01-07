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
// mandelbrot boundaries in x: -2.00 e 0.47
// mandelbrot boundaries in y: -1.12 e 1.12
const double MIN_X = -2.00;
const double MAX_X = 0.47;
const double MIN_Y = -1.12;
const double MAX_Y = 1.12;
const double ZOOM_SQUARE_WIDTH = 300;
const int START_MAX_ITERATIONS = 50;
const bool START_COLORED = false;
const bool START_ZOOMING = false;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct HueControl {
    Vector2 pos;
    int radius;
    float value;
    Color color;
} HueControl;

typedef struct ColorBar {
    Vector2 pos;
    int width;
    int height;
    int increment;
    HueControl *hueControlStart;
    HueControl *hueControlEnd;
} ColorBar;

/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
ColorBar colorBar;
HueControl hueControlStart;
HueControl hueControlEnd;
HueControl *selectedHueControl = NULL;
HueControl *otherHueControl = NULL;

int xPress;
int yPress;
int xOffset;
int yOffset;

double minX;
double maxX;
double minY;
double maxY;

int maxIterations;
bool colored;
bool zooming;

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
void drawColorBar( const ColorBar *colorBar );
void drawHueControlBar( const HueControl *hueControl );
bool interceptsHueControlCoord( const HueControl *hueControl, int x, int y );

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 800;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Fractal de Mandelbrot" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    colorBar = (ColorBar) {
        .pos = {
            .x = 20,
            .y = 20
        },
        .width = 756,
        .height = 20,
        .increment = 10
    };

    hueControlStart = (HueControl) {
        .pos = {
            .x = colorBar.pos.x,
            .y = colorBar.pos.y + colorBar.height
        },
        .radius = 5,
        .value = 0,
        .color = BLACK
    };

    hueControlEnd = (HueControl) {
        .pos = {
            .x = colorBar.pos.x + colorBar.width,
            .y = colorBar.pos.y + colorBar.height
        },
        .radius = 5,
        .value = 360,
        .color = BLACK
    };

    colorBar.hueControlStart = &hueControlStart;
    colorBar.hueControlEnd = &hueControlEnd;

    minX = MIN_X;
    maxX = MAX_X;
    minY = MIN_Y;
    maxY = MAX_Y;

    maxIterations = START_MAX_ITERATIONS;
    colored = START_COLORED;

    while ( !WindowShouldClose() ) {
        inputAndUpdate();
        draw();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( void ) {

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        xPress = GetMouseX();
        yPress = GetMouseY();
        if ( interceptsHueControlCoord( &hueControlStart, xPress, yPress ) ) {
            xOffset = xPress - hueControlStart.pos.x;
            yOffset = yPress - hueControlStart.pos.y;
            selectedHueControl = &hueControlStart;
            otherHueControl = &hueControlEnd;
        } else if ( interceptsHueControlCoord( &hueControlEnd, xPress, yPress ) ) {
            xOffset = xPress - hueControlEnd.pos.x;
            yOffset = yPress - hueControlEnd.pos.y;
            selectedHueControl = &hueControlEnd;
            otherHueControl = &hueControlStart;
        } else if ( zooming ) {
            minX = Lerp( minX, maxX, (GetMouseX() - ZOOM_SQUARE_WIDTH / 2) / GetScreenWidth() );
            maxX = Lerp( minX, maxX, (GetMouseX() + ZOOM_SQUARE_WIDTH / 2) / GetScreenWidth() );
            minY = Lerp( minY, maxY, (GetMouseY() - ZOOM_SQUARE_WIDTH / 2) / GetScreenWidth() );
            maxY = Lerp( minY, maxY, (GetMouseY() + ZOOM_SQUARE_WIDTH / 2) / GetScreenWidth() );
        }
    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        selectedHueControl = NULL;
        otherHueControl = NULL;
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && selectedHueControl ) {
        selectedHueControl->pos.x = GetMouseX() - xOffset;
        if ( otherHueControl == &hueControlEnd ) {
            if ( selectedHueControl->pos.x < colorBar.pos.x ) {
                selectedHueControl->pos.x = colorBar.pos.x;
            } else if ( selectedHueControl->pos.x > otherHueControl->pos.x ) {
                selectedHueControl->pos.x = otherHueControl->pos.x;
            }
        } else if ( otherHueControl == &hueControlStart ) {
            if ( selectedHueControl->pos.x > colorBar.pos.x + colorBar.width ) {
                selectedHueControl->pos.x = colorBar.pos.x + colorBar.width;
            } else if ( selectedHueControl->pos.x < otherHueControl->pos.x ) {
                selectedHueControl->pos.x = otherHueControl->pos.x;
            }
        }
    }

    if ( IsKeyPressed( KEY_UP ) ) {
        maxIterations += 10;
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        maxIterations -= 10;
        if ( maxIterations < 0 ) {
            maxIterations = 0;
        }
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        colored = !colored;
    }

    if ( IsKeyPressed( KEY_Z ) ) {
        zooming = !zooming;
    }

    hueControlStart.value = Lerp( 0, 360, ( hueControlStart.pos.x - colorBar.pos.x ) / colorBar.width );
    hueControlEnd.value = Lerp( 0, 360, ( hueControlEnd.pos.x - colorBar.pos.x ) / colorBar.width );

}

void draw( void ) {

    BeginDrawing();
    ClearBackground( WHITE );

    // based on https://en.wikipedia.org/wiki/Mandelbrot_set
    
    Color color = { 0, 0, 0, 255 };

    for ( int i = 0; i < GetScreenHeight(); i++ ) {
        for ( int j = 0; j < GetScreenWidth(); j++ ) {

            double px = j;
            double py = i;
            double x = 0.0;
            double y = 0.0;

            /*double x0 = Lerp( minX, maxX, ( (double) j / (GetScreenWidth()-1) ) );
            double y0 = Lerp( minY, maxY, ( (double) i / (GetScreenHeight()-1) ) );*/
            double x0 = Lerp( minX, maxX, ( px / GetScreenWidth() ) );
            double y0 = Lerp( minY, maxY, ( py / GetScreenHeight() ) );
            
            int k;
            for ( k = 0; k < maxIterations && x*x + y*y <= 2*2; k++ ) {
                double xt = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xt;
            }

            if ( colored ) {
                color = ColorFromHSV( 
                    hueControlStart.value + ( hueControlEnd.value - hueControlStart.value ) * ( k / (float) maxIterations ), 1, 0.7 );
            } else {
                int c = 255 - 255 * ( k / (float) maxIterations );
                color.r = c;
                color.g = c;
                color.b = c;
            }

            DrawPixel( px, py, color );

        }
    }

    if ( zooming ) {
        DrawRectangleLines( 
            GetMouseX() - ZOOM_SQUARE_WIDTH / 2, 
            GetMouseY() - ZOOM_SQUARE_WIDTH / 2, 
            ZOOM_SQUARE_WIDTH, 
            ZOOM_SQUARE_WIDTH, 
            BLACK );
    }

    if ( colored ) {
        drawColorBar( &colorBar );
    }

    EndDrawing();

}

void drawColorBar( const ColorBar *colorBar ) {

    int quant = 360 / colorBar->increment;
    int t_width = colorBar->width / quant;

    for ( int i = 0; i < quant; i++ ) {
        DrawRectangleGradientH( 
            colorBar->pos.x + i * t_width, 
            colorBar->pos.y, 
            t_width, 
            colorBar->height, 
            ColorFromHSV( i * colorBar->increment, 1, 1 ),
            ColorFromHSV( (i+1) * colorBar->increment, 1, 1 ) );
    }

    DrawRectangleLines( colorBar->pos.x, colorBar->pos.y, 
                        colorBar->width, colorBar->height, 
                        WHITE );

    drawHueControlBar( colorBar->hueControlStart );
    drawHueControlBar( colorBar->hueControlEnd );

}

void drawHueControlBar( const HueControl *hueControl ) {
    DrawCircle( hueControl->pos.x, hueControl->pos.y, hueControl->radius, hueControl->color );
    DrawCircleLines( hueControl->pos.x, hueControl->pos.y, hueControl->radius, WHITE );
}

bool interceptsHueControlCoord( const HueControl *hueControl, int x, int y ) {
    float c1 = x - hueControl->pos.x;
    float c2 = y - hueControl->pos.y;
    return c1 * c1 + c2 * c2 <= hueControl->radius * hueControl->radius;
}
