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

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/
#define MAX_ZOOM 15

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
// mandelbrot boundaries in x: -2.00 e 0.47
// mandelbrot boundaries in y: -1.12 e 1.12
const double MIN_X = -2.00;
const double MAX_X = 0.47;
const double MIN_Y = -1.12;
const double MAX_Y = 1.12;
const int SCREENS_SIZE = 800;
const double ZOOM_SQUARE_SIZE = 200;
const int START_MAX_ITERATIONS = 50;
const bool START_MANDELBROT = true;
const bool START_COLORED = true;
const bool START_GRADIENT = false;
const bool START_ZOOMING = false;

const double MIN_HUE = 0;
const double MAX_HUE = 360;
const double COMPLEX_REAL_IMAGINARY_LIMIT = 2;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct SliderControl {
    Vector2 pos;
    int radius;
    double value;
    Color color;
} SliderControl;

typedef struct ColorBar {
    Vector2 pos;
    int width;
    int height;
    int increment;
    SliderControl *hueControlStart;
    SliderControl *hueControlEnd;
} ColorBar;

typedef struct ComplexBar {
    Vector2 pos;
    int width;
    int height;
    int increment;
    Color color;
    SliderControl *complexControlReal;
    SliderControl *complexControlImaginary;
} ComplexBar;

/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
ColorBar colorBar;
SliderControl hueControlStart;
SliderControl hueControlEnd;
SliderControl *selectedHueControl = NULL;
SliderControl *otherHueControl = NULL;

ComplexBar complexBar;
SliderControl complexControlReal;
SliderControl complexControlImaginary;
SliderControl *selectedComplexControl = NULL;

int xPress;
int yPress;
int xOffset;
int yOffset;

double minX;
double maxX;
double minY;
double maxY;

bool mandelbrot;
bool colored;
bool gradient;
bool zooming;
int maxIterations;

double cx;
double cy;
double scapeRadius;

double lastMinX[MAX_ZOOM];
double lastMaxX[MAX_ZOOM];
double lastMinY[MAX_ZOOM];
double lastMaxY[MAX_ZOOM];
int currentZoom;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( void );
int getIteration( double x0, double y0, double x, double y, int maxIterations );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( void );
void drawColorBar( const ColorBar *colorBar );
void drawComplexBar( const ComplexBar *complexBar );
void drawSliderControl( const SliderControl *sliderControl );
bool interceptsSliderControlCoord( const SliderControl *sliderControl, int x, int y );

int main( void ) {

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( SCREENS_SIZE, SCREENS_SIZE, "Fractais de Mandelbrot e Julia" );
    InitAudioDevice();
    SetTargetFPS( 60 );

    colorBar = (ColorBar) {
        .pos = {
            .x = 20,
            .y = 20
        },
        .width = GetScreenWidth() - 40,
        .height = 20,
        .increment = 1
    };

    hueControlStart = (SliderControl) {
        .pos = {
            .x = colorBar.pos.x,
            .y = colorBar.pos.y + colorBar.height
        },
        .radius = 5,
        .value = MIN_HUE,
        .color = BLACK
    };

    hueControlEnd = (SliderControl) {
        .pos = {
            .x = colorBar.pos.x + colorBar.width,
            .y = colorBar.pos.y + colorBar.height
        },
        .radius = 5,
        .value = MAX_HUE,
        .color = BLACK
    };

    colorBar.hueControlStart = &hueControlStart;
    colorBar.hueControlEnd = &hueControlEnd;

    complexBar = (ComplexBar) {
        .pos = {
            .x = 20,
            .y = GetScreenHeight() - 40
        },
        .width = GetScreenWidth() - 40,
        .height = 20,
        .color = BLUE,
        .increment = 0
    };

    complexControlReal = (SliderControl) {
        .pos = {
            .x = complexBar.pos.x,
            .y = complexBar.pos.y
        },
        .radius = 5,
        .value = 0.4,
        .color = BLACK
    };

    complexControlImaginary = (SliderControl) {
        .pos = {
            .x = complexBar.pos.x,
            .y = complexBar.pos.y + complexBar.height
        },
        .radius = 5,
        .value = 0.4,
        .color = BLACK
    };

    complexBar.complexControlReal = &complexControlReal;
    complexBar.complexControlImaginary = &complexControlImaginary;

    complexControlReal.pos.x = (int) Lerp( 
        complexBar.pos.x, complexBar.pos.x + complexBar.width, 
        (complexControlReal.value + COMPLEX_REAL_IMAGINARY_LIMIT) / ( COMPLEX_REAL_IMAGINARY_LIMIT * 2 ) );
    complexControlImaginary.pos.x = (int) Lerp( 
        complexBar.pos.x, complexBar.pos.x + complexBar.width, 
        (complexControlImaginary.value + COMPLEX_REAL_IMAGINARY_LIMIT) / ( COMPLEX_REAL_IMAGINARY_LIMIT * 2 ) );

    minX = MIN_X;
    maxX = MAX_X;
    minY = MIN_Y;
    maxY = MAX_Y;

    mandelbrot = START_MANDELBROT;
    colored = START_COLORED;
    gradient = START_GRADIENT;
    maxIterations = START_MAX_ITERATIONS;
    
    cx = complexControlReal.value;
    cy = complexControlImaginary.value;
    scapeRadius = 2;

    currentZoom = 0;

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

        if ( interceptsSliderControlCoord( &hueControlStart, xPress, yPress ) ) {
            xOffset = xPress - hueControlStart.pos.x;
            yOffset = yPress - hueControlStart.pos.y;
            selectedHueControl = &hueControlStart;
            otherHueControl = &hueControlEnd;
        } else if ( interceptsSliderControlCoord( &hueControlEnd, xPress, yPress ) ) {
            xOffset = xPress - hueControlEnd.pos.x;
            yOffset = yPress - hueControlEnd.pos.y;
            selectedHueControl = &hueControlEnd;
            otherHueControl = &hueControlStart;
        } else if ( interceptsSliderControlCoord( &complexControlReal, xPress, yPress ) ) {
            xOffset = xPress - complexControlReal.pos.x;
            yOffset = yPress - complexControlReal.pos.y;
            selectedComplexControl = &complexControlReal;
        } else if ( interceptsSliderControlCoord( &complexControlImaginary, xPress, yPress ) ) {
            xOffset = xPress - complexControlImaginary.pos.x;
            yOffset = yPress - complexControlImaginary.pos.y;
            selectedComplexControl = &complexControlImaginary;
        }
    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        selectedHueControl = NULL;
        otherHueControl = NULL;
        selectedComplexControl = NULL;
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        if ( selectedHueControl ) {
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
        } else if ( selectedComplexControl ) {
            selectedComplexControl->pos.x = GetMouseX() - xOffset;
            if ( selectedComplexControl->pos.x < complexBar.pos.x ) {
                selectedComplexControl->pos.x = complexBar.pos.x;
            } else if ( selectedComplexControl->pos.x > complexBar.pos.x + complexBar.width ) {
                selectedComplexControl->pos.x = complexBar.pos.x + complexBar.width;
            }
        }
    }

    int wheelMove = GetMouseWheelMove();

    if ( wheelMove > 0 ) {

        if ( currentZoom < MAX_ZOOM ) {

            lastMinX[currentZoom] = minX;
            lastMaxX[currentZoom] = maxX;
            lastMinY[currentZoom] = minY;
            lastMaxY[currentZoom] = maxY;
            currentZoom++;
            printf( "%d\n", currentZoom );

            double xStart = GetMouseX() - ZOOM_SQUARE_SIZE / 2;
            double xEnd = GetMouseX() + ZOOM_SQUARE_SIZE / 2;
            double yStart = GetMouseY() - ZOOM_SQUARE_SIZE / 2;
            double yEnd = GetMouseY() + ZOOM_SQUARE_SIZE / 2;

            if ( xStart < 0 ) {
                xEnd += -xStart;
                xStart = 0;
            } else if ( xEnd > GetScreenWidth() ) {
                xStart -= xEnd - GetScreenWidth();
                xEnd = GetScreenWidth();
            }

            if ( yStart < 0 ) {
                yEnd += -yStart;
                yStart = 0;
            } else if ( yEnd > GetScreenHeight() ) {
                yStart -= yEnd - GetScreenHeight();
                yEnd = GetScreenWidth();
            }

            double nMinX = Lerp( minX, maxX, xStart / GetScreenWidth() );
            double nMaxX = Lerp( minX, maxX, xEnd / GetScreenWidth() );
            double nMinY = Lerp( minY, maxY, yStart / GetScreenHeight() );
            double nMaxY = Lerp( minY, maxY, yEnd / GetScreenHeight() );
            minX = nMinX;
            maxX = nMaxX;
            minY = nMinY;
            maxY = nMaxY;

        }

    } else if ( wheelMove < 0 ) {
        printf( "xxx" );
        if ( currentZoom > 0 ) {
            currentZoom--;
            minX = lastMinX[currentZoom];
            maxX = lastMaxX[currentZoom];
            minY = lastMinY[currentZoom];
            maxY = lastMaxY[currentZoom];
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

    if ( IsKeyPressed( KEY_C ) ) {
        colored = !colored;
    }

    if ( IsKeyPressed( KEY_G ) ) {
        gradient = !gradient;
    }

    if ( IsKeyPressed( KEY_M ) ) {
        minX = MIN_X;
        maxX = MAX_X;
        minY = MIN_Y;
        maxY = MAX_Y;
        currentZoom = 0;
        mandelbrot = true;
    }

    if ( IsKeyPressed( KEY_J ) ) {
        minX = -scapeRadius;
        maxX = scapeRadius;
        minY = -scapeRadius;
        maxY = scapeRadius;
        currentZoom = 0;
        mandelbrot = false;
    }

    if ( IsKeyPressed( KEY_Z ) ) {
        zooming = !zooming;
    }

    if ( IsKeyPressed( KEY_R ) ) {
        if ( mandelbrot ) {
            minX = MIN_X;
            maxX = MAX_X;
            minY = MIN_Y;
            maxY = MAX_Y;
        } else {
            minX = -scapeRadius;
            maxX = scapeRadius;
            minY = -scapeRadius;
            maxY = scapeRadius;
        }
        currentZoom = 0;
    }

    hueControlStart.value = Lerp( MIN_HUE, MAX_HUE, ( hueControlStart.pos.x - colorBar.pos.x ) / colorBar.width );
    hueControlEnd.value = Lerp( MIN_HUE, MAX_HUE, ( hueControlEnd.pos.x - colorBar.pos.x ) / colorBar.width );
    complexControlReal.value = Lerp( -COMPLEX_REAL_IMAGINARY_LIMIT, COMPLEX_REAL_IMAGINARY_LIMIT, ( complexControlReal.pos.x - complexBar.pos.x ) / complexBar.width );
    complexControlImaginary.value = Lerp( -COMPLEX_REAL_IMAGINARY_LIMIT, COMPLEX_REAL_IMAGINARY_LIMIT, ( complexControlImaginary.pos.x - complexBar.pos.x ) / complexBar.width );

    cx = complexControlReal.value;
    cy = complexControlImaginary.value;

}

void draw( void ) {

    BeginDrawing();
    ClearBackground( WHITE );

    // based on https://en.wikipedia.org/wiki/Mandelbrot_set
    //          https://en.wikipedia.org/wiki/Julia_set
    
    Color color = { 0, 0, 0, 255 };

    for ( int i = 0; i < GetScreenHeight(); i++ ) {
        for ( int j = 0; j < GetScreenWidth(); j++ ) {

            double px = j;
            double py = i;
            double x = 0.0;
            double y = 0.0;
            double x0;
            double y0;
            double xTemp;
            int iteration;

            if ( mandelbrot ) {

                // fixed complex number
                x0 = Lerp( minX, maxX, ( px / GetScreenWidth() ) );   // real
                y0 = Lerp( minY, maxY, ( py / GetScreenHeight() ) );  // imaginary

                for ( iteration = 0; 
                      iteration < maxIterations && 
                      x*x + y*y <= ( 1 << 16 ); 
                      iteration++ ) {
                    xTemp = x * x - y * y + x0;
                    y = 2 * x * y + y0;
                    x = xTemp;
                }

                if ( colored ) {

                    if ( gradient ) {

                        double diff = 0;
                        if ( iteration < maxIterations ) {
                            double logZn = log( x * x + y * y ) / 2;
                            double nu = log( logZn / log(2) ) / log(2);
                            diff = iteration - 1 - nu;
                        }

                        double vStart = diff;
                        double vEnd = diff;
                        /*double vStart = diff;
                        double vEnd = iteration;
                        double vStart = iteration;
                        double vEnd = diff;
                        double vStart = iteration;
                        double vEnd = iteration;*/

                        Color color1 = ColorFromHSV( 
                                hueControlStart.value + 
                                ( hueControlEnd.value - hueControlStart.value ) * 
                                ( ((int) vStart) / (double) maxIterations ), 
                                1, 0.7 );
                        Color color2 = ColorFromHSV( 
                                hueControlStart.value + 
                                ( hueControlEnd.value - hueControlStart.value ) * 
                                ( ((int) (vEnd+1)) / (double) maxIterations ), 
                                1, 0.7 );

                        diff = diff - ((long)diff);

                        color = (Color) {
                            .r = Lerp( color1.r, color2.r, diff ),
                            .g = Lerp( color1.g, color2.g, diff ),
                            .b = Lerp( color1.b, color2.b, diff ),
                            .a = 255
                        };

                    } else {

                        color = ColorFromHSV( hueControlStart.value + 
                            ( hueControlEnd.value - hueControlStart.value ) * 
                            ( iteration / (double) maxIterations ), 
                            1, 0.7 );

                    }

                } else {

                    int c;
                    
                    if ( gradient ) {

                        double diff = 0;
                        if ( iteration < maxIterations ) {
                            double logZn = log( x * x + y * y ) / 2;
                            double nu = log( logZn / log(2) ) / log(2);
                            diff = iteration - 1 - nu;
                        }
                        
                        double c1 = 255 * (diff) / ( (double) maxIterations );
                        double c2 = 255 * (diff+1) / ( (double) maxIterations );
                        diff = diff - ((long)diff);

                        c = Lerp( c1, c2, diff );

                    } else {
                        c = 255 - 255 * ( iteration / (double) maxIterations );
                    }

                    color.r = c;
                    color.g = c;
                    color.b = c;

                }

            } else {

                // variyng complex number (min and max related to scapeRadius)
                x0 = Lerp( minX, maxX, ( px / GetScreenWidth() ) );    // real
                y0 = Lerp( minY, maxY, ( py / GetScreenHeight() ) );   // imaginary
                
                for ( iteration = 0; 
                      iteration < maxIterations && 
                      x0*x0 + y0*y0 < scapeRadius*scapeRadius; 
                      iteration++ ) {
                    xTemp = x0 * x0 - y0 * y0;
                    y0 = 2 * x0 * y0 + cy;
                    x0 = xTemp + cx;
                }

                if ( colored ) {

                    if ( gradient ) {

                        double diff = 0;
                        if ( iteration < maxIterations ) {
                            double logZn = log( x0 * x0 + y0 * y0 ) / 2;
                            double nu = log( logZn / log(2) ) / log(2);
                            diff = iteration - 1 - nu;
                        }

                        double vStart = diff;
                        double vEnd = diff;

                        Color color1 = ColorFromHSV( 
                                hueControlStart.value + 
                                ( hueControlEnd.value - hueControlStart.value ) * 
                                ( ((int) vStart) / (double) maxIterations ), 
                                1, 0.7 );
                        Color color2 = ColorFromHSV( 
                                hueControlStart.value + 
                                ( hueControlEnd.value - hueControlStart.value ) * 
                                ( ((int) (vEnd+1)) / (double) maxIterations ), 
                                1, 0.7 );

                        diff = diff - ((long)diff);

                        color = (Color) {
                            .r = Lerp( color1.r, color2.r, diff ),
                            .g = Lerp( color1.g, color2.g, diff ),
                            .b = Lerp( color1.b, color2.b, diff ),
                            .a = 255
                        };

                    } else {

                        color = ColorFromHSV( hueControlStart.value + 
                            ( hueControlEnd.value - hueControlStart.value ) * 
                            ( iteration / (double) maxIterations ), 
                            1, 0.7 );

                    }

                } else {

                    int c;
                    
                    if ( gradient ) {

                        double diff = 0;
                        if ( iteration < maxIterations ) {
                            double logZn = log( x0 * x0 + y0 * y0 ) / 2;
                            double nu = log( logZn / log(2) ) / log(2);
                            diff = iteration - 1 - nu;
                        }
                        
                        double c1 = 255 * (diff) / ( (double) maxIterations );
                        double c2 = 255 * (diff+1) / ( (double) maxIterations );
                        diff = diff - ((long)diff);

                        c = Lerp( c1, c2, diff );

                    } else {
                        c = 255 - 255 * ( iteration / (double) maxIterations );
                    }

                    color.r = c;
                    color.g = c;
                    color.b = c;

                }

            }

            DrawPixel( px, py, color );

        }
    }

    if ( zooming ) {
        DrawRectangleLines( 
            GetMouseX() - ZOOM_SQUARE_SIZE / 2, 
            GetMouseY() - ZOOM_SQUARE_SIZE / 2, 
            ZOOM_SQUARE_SIZE, 
            ZOOM_SQUARE_SIZE, 
            BLACK );
    }

    if ( colored ) {
        drawColorBar( &colorBar );
    }

    if ( !mandelbrot ) {
        drawComplexBar( &complexBar );
    }

    DrawFPS( 20, GetScreenHeight() - 60 );
    EndDrawing();

}

void drawColorBar( const ColorBar *colorBar ) {

    int quant = 10;
    int increment = 360 / quant;
    int t_width = colorBar->width / quant;

    for ( int i = 0; i < quant; i++ ) {
        DrawRectangleGradientH( 
            colorBar->pos.x + i * t_width, 
            colorBar->pos.y, 
            t_width, 
            colorBar->height, 
            ColorFromHSV( i * increment, 1, 1 ),
            ColorFromHSV( (i+1) * increment, 1, 1 ) );
    }

    DrawRectangleLines( colorBar->pos.x, colorBar->pos.y, 
                        colorBar->width, colorBar->height, 
                        WHITE );

    drawSliderControl( colorBar->hueControlStart );
    drawSliderControl( colorBar->hueControlEnd );

}

void drawComplexBar( const ComplexBar *complexBar ) {
    DrawRectangle( complexBar->pos.x, complexBar->pos.y, complexBar->width, complexBar->height, complexBar->color );
    DrawRectangleLines( complexBar->pos.x, complexBar->pos.y, complexBar->width, complexBar->height, WHITE );
    drawSliderControl( complexBar->complexControlReal );
    drawSliderControl( complexBar->complexControlImaginary );
}

void drawSliderControl( const SliderControl *sliderControl ) {
    DrawCircle( sliderControl->pos.x, sliderControl->pos.y, sliderControl->radius, sliderControl->color );
    DrawCircleLines( sliderControl->pos.x, sliderControl->pos.y, sliderControl->radius, WHITE );
    DrawText( TextFormat( "%.4f", sliderControl->value ), sliderControl->pos.x + sliderControl->radius, sliderControl->pos.y + 5, 10, BLACK );
}

bool interceptsSliderControlCoord( const SliderControl *sliderControl, int x, int y ) {
    double c1 = x - sliderControl->pos.x;
    double c2 = y - sliderControl->pos.y;
    return c1 * c1 + c2 * c2 <= sliderControl->radius * sliderControl->radius;
}
