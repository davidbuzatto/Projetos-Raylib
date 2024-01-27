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
#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#undef RAYGUI_IMPLEMENTATION

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
typedef struct Ball {
    Vector2 pos;
    float radius;
    float segments;
    float strokeWidth;
    Color strokeColor;
    Color fillColor;
    float strokeColorAlpha;
    float fillColorAlpha;
} Ball;

typedef struct ControlWindow {
    Rectangle rect;
    int marginTop;
    int marginLeft;
    int titleBarHeight;
    bool visible;
    bool moving;
    Ball *ball;
} ControlWindow;

typedef struct GameWorld {
    Ball ball;
    ControlWindow controlWindow;
    bool drawGrid;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
int xPress;
int yPress;
int xOffset;
int yOffset;


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
void draw( GameWorld *gw );
void drawBall( Ball* ball );
void drawControlWindow( ControlWindow *cw );
bool checkCollisionControlWindowMouse( ControlWindow *c2, Vector2 mousePos );

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

    const int screenWidth = 1000;
    const int screenHeight = 800;

    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Bolinha GUI" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    Ball *ball = &(gw->ball);
    ControlWindow *cw = &(gw->controlWindow);

    if ( IsKeyPressed( KEY_SPACE ) ) {
        cw->visible = !cw->visible;
    }

    if ( IsKeyPressed( KEY_ENTER ) ) {
        gw->drawGrid = !gw->drawGrid;
    }

    if ( cw->visible && IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        Vector2 p = GetMousePosition();
        if ( checkCollisionControlWindowMouse( cw, p ) ) {
            cw->moving = true;
            xPress = p.x;
            yPress = p.y;
            xOffset = cw->rect.x - xPress;
            yOffset = cw->rect.y - yPress;
        }
    }

    if ( cw->moving ) {
        Vector2 p = GetMousePosition();
        cw->rect.x = p.x + xOffset;
        cw->rect.y = p.y + yOffset;
    }

    if ( cw->moving && IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        cw->moving = false;
    }

    if ( ball->strokeWidth > ball->radius ) {
        ball->strokeWidth = ball->radius;
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    if ( gw->drawGrid ) {
        int cellWidth = 50;
        for ( int i = 0; i < GetScreenHeight() / cellWidth; i++ ) {
            for ( int j = 0; j < GetScreenWidth() / cellWidth; j++ ) {
                if ( i % 2 == 0 ) {
                    if ( j % 2 == 0 ) {
                        DrawRectangle( j*cellWidth, i*cellWidth, cellWidth, cellWidth, LIGHTGRAY );
                    }
                } else {
                    if ( j % 2 != 0 ) {
                        DrawRectangle( j*cellWidth, i*cellWidth, cellWidth, cellWidth, LIGHTGRAY );
                    }
                }
            }
        }
    }

    drawBall( &(gw->ball) );
    drawControlWindow( &(gw->controlWindow) );

    EndDrawing();

}

void drawBall( Ball* ball ) {

    ball->strokeColor.a = Lerp( 0, 255, ball->strokeColorAlpha );
    ball->fillColor.a = Lerp( 0, 255, ball->fillColorAlpha );

    DrawCircleSector( ball->pos, ball->radius - ball->strokeWidth, 0, 360, ball->segments, ball->fillColor );
    DrawRing( ball->pos, ball->radius - ball->strokeWidth, ball->radius, 0, 360, ball->segments, ball->strokeColor );

}

void drawControlWindow( ControlWindow *cw ) {

    if ( cw->visible ) {

        if ( GuiWindowBox( cw->rect, "Propriedades da Bolinha" ) == 1 ) {
            cw->visible = false;
        }

        GuiSlider( (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop, 200, 20 }, "raio:", TextFormat( "%2.2f", cw->ball->radius ), &(cw->ball->radius), 10, 300 );
        GuiSlider( (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 25, 200, 20 }, "segmentos:", TextFormat( "%2.2f", cw->ball->segments ), &(cw->ball->segments), 4, 100 );
        GuiSlider( (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 50, 200, 20 }, "contorno:", TextFormat( "%2.2f", cw->ball->strokeWidth ), &(cw->ball->strokeWidth), 0, cw->ball->radius );

        Rectangle rStrokeColor = (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 75, 200, 200 };
        Rectangle rStrokeAlpha = (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 280, 200, 20 };
        GuiColorPicker( rStrokeColor, NULL, &(cw->ball->strokeColor) );
        GuiColorBarAlpha( rStrokeAlpha, "transparência", &(cw->ball->strokeColorAlpha) );

        Rectangle rFillColor = (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 305, 200, 200 };
        Rectangle rFillAlpha = (Rectangle){ cw->rect.x + cw->marginLeft, cw->rect.y + cw->marginTop + 510, 200, 20 };
        GuiColorPicker( rFillColor, NULL, &(cw->ball->fillColor) );
        GuiColorBarAlpha( rFillAlpha, "transparência", &(cw->ball->fillColorAlpha) );

        const char *strokeColorLabelText = "cor contorno: ";
        const char *fillColorLabelText = "cor preenchimento: ";
        const char *alphaLabelText = "transparência: ";
        int wStrokeColorLabelText = MeasureText( strokeColorLabelText, 10 );
        int wFillColorLabelText = MeasureText( fillColorLabelText, 10 );
        int wAlphaLabelText = MeasureText( alphaLabelText, 10 );
        GuiLabel( (Rectangle){ rStrokeColor.x - wStrokeColorLabelText - 2, rStrokeColor.y, wStrokeColorLabelText, 20 }, strokeColorLabelText );
        GuiLabel( (Rectangle){ rStrokeAlpha.x - wAlphaLabelText - 2, rStrokeAlpha.y, wAlphaLabelText, 20 }, alphaLabelText );
        GuiLabel( (Rectangle){ rFillColor.x - wFillColorLabelText - 2, rFillColor.y, wFillColorLabelText, 20 }, fillColorLabelText );
        GuiLabel( (Rectangle){ rFillAlpha.x - wAlphaLabelText - 2, rFillAlpha.y, wAlphaLabelText, 20 }, alphaLabelText );

    }

}

bool checkCollisionControlWindowMouse( ControlWindow *cw, Vector2 mousePos ) {
    Rectangle titleBarRect = {
        .x = cw->rect.x,
        .y = cw->rect.y,
        .width = cw->rect.width,
        .height = cw->titleBarHeight
    };
    return CheckCollisionPointRec( mousePos, titleBarRect );
}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    gw = (GameWorld) {
        .ball = (Ball){
            .pos = { 
                .x = GetScreenWidth() / 2,
                .y = GetScreenHeight() / 2
            },
            .radius = 50,
            .segments = 36,
            .strokeWidth = 1,
            .strokeColor = GetColor( 0x000000ff ),
            .fillColor = GetColor( 0xffffffff ),
            .strokeColorAlpha = 1,
            .fillColorAlpha = 1
        },
        .controlWindow = (ControlWindow){
            .rect = {
                .x = 20,
                .y = 20,
                .width = 350,
                .height = 570
            },
            .marginTop = 30,
            .marginLeft = 110,
            .titleBarHeight = 25,
            .visible = true,
            .moving = false,
            .ball = NULL
        },
        .drawGrid = true
    };

    gw.controlWindow.ball = &(gw.ball);

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