/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Ball movement simulation with ball properties changed by immediate
 * mode GUI (raygui and Raylib).
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
#define MAX_BALLS 200

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
float INITIAL_GRAVITY = 100;

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef struct Ball {
    Vector2 pos;
    Vector2 vel;
    float friction;
    float elasticity;
    float radius;
    float segments;
    float strokeWidth;
    Color strokeColor;
    Color fillColor;
    float strokeColorAlpha;
    float fillColorAlpha;
    bool dragging;
} Ball;

typedef struct BallControlWindow {
    Rectangle rect;
    int marginTop;
    int marginLeft;
    int titleBarHeight;
    bool visible;
    bool dragging;
    Ball *ball;
} BallControlWindow;

typedef struct BallPropertyWindow {
    Rectangle rect;
    int marginTop;
    int marginLeft;
    int titleBarHeight;
    bool visible;
    bool dragging;
    Ball *ball;
} BallPropertyWindow;

typedef struct GameWorldControlWindow {
    Rectangle rect;
    int marginTop;
    int marginLeft;
    int titleBarHeight;
    bool visible;
    bool dragging;
    struct GameWorld *gw;
} GameWorldControlWindow;

typedef struct GameWorld {
    Ball *currentBall;
    Ball *balls[MAX_BALLS];
    int ballQuantity;
    int maxBalls;
    BallControlWindow *ballControlWindow;
    BallPropertyWindow *ballPropertyWindow;
    GameWorldControlWindow *controlWindow;
    float gravity;
    bool drawGrid;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Ball ball;
BallControlWindow bcw;
BallPropertyWindow bpw;
GameWorldControlWindow gwcw;

int xPress;
int yPress;
int xOffset;
int yOffset;
Vector2 prevMousePos;


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
void moveBall( Ball* ball, GameWorld *gw );
void resetBall( Ball *ball );
void newBall( GameWorld *gw, Vector2 mousePos );
void resetGameWorld( GameWorld *gw );
void drawBallControlWindow( BallControlWindow *bcw );
void drawBallPropertyWindow( BallPropertyWindow *bpw );
void drawGameWorldControlWindow( GameWorldControlWindow *gwcw );
bool checkCollisionBallMouse( Ball *ball, Vector2 mousePos );
bool checkCollisionBallControlWindowMouse( BallControlWindow *bcw, Vector2 mousePos );
bool checkCollisionBallPropertyWindowMouse( BallPropertyWindow *bpw, Vector2 mousePos );
bool checkCollisionGameWorldControlWindowMouse( GameWorldControlWindow *gwcw, Vector2 mousePos );

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
    SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    InitWindow( screenWidth, screenHeight, "Bolinhas GUI" );
    SetExitKey( 0 );
    SetTargetFPS( 60 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    BallControlWindow *bcw = gw->ballControlWindow;
    BallPropertyWindow *bpw = gw->ballPropertyWindow;
    GameWorldControlWindow *gwcw = gw->controlWindow;
    Vector2 mousePos = GetMousePosition();

    if ( IsKeyPressed( KEY_R ) ) {
        resetGameWorld( gw );
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        for ( int i = 0; i < gw->ballQuantity; i++ ) {
            Ball *b = gw->balls[i];
            b->dragging = false;
            b->vel.x = GetRandomValue( -2000, 2000 );
            b->vel.y = GetRandomValue( -2000, 2000 );
            GuiEnable();
        }
    }

    if ( IsKeyPressed( KEY_ESCAPE ) ) {
        bcw->visible = false;
        bpw->visible = false;
        gwcw->visible = false;
    }

    if ( IsKeyPressed( KEY_ONE ) ) {
        bcw->visible = true;
    }

    if ( IsKeyPressed( KEY_TWO ) ) {
        bpw->visible = true;
    }

    if ( IsKeyPressed( KEY_THREE ) ) {
        gwcw->visible = true;
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        if ( bcw->visible && checkCollisionBallControlWindowMouse( bcw, mousePos ) ) {
            bcw->dragging = true;
            xPress = mousePos.x;
            yPress = mousePos.y;
            xOffset = bcw->rect.x - xPress;
            yOffset = bcw->rect.y - yPress;
        } else if ( bpw->visible && checkCollisionBallPropertyWindowMouse( bpw, mousePos ) ) {
            bpw->dragging = true;
            xPress = mousePos.x;
            yPress = mousePos.y;
            xOffset = bpw->rect.x - xPress;
            yOffset = bpw->rect.y - yPress;
        } else if ( gwcw->visible && checkCollisionGameWorldControlWindowMouse( gwcw, mousePos ) ) {
            gwcw->dragging = true;
            xPress = mousePos.x;
            yPress = mousePos.y;
            xOffset = gwcw->rect.x - xPress;
            yOffset = gwcw->rect.y - yPress;
        } else {
            for ( int i = gw->ballQuantity-1; i >= 0; i-- ) {
                Ball *b = gw->balls[i];
                if ( checkCollisionBallMouse( b, mousePos ) ) {
                    b->dragging = true;
                    xPress = mousePos.x;
                    yPress = mousePos.y;
                    xOffset = b->pos.x - xPress;
                    yOffset = b->pos.y - yPress;
                    gw->currentBall = b;
                    GuiDisable();
                    break;
                }
            }
        }
    }

    Ball *ball = gw->currentBall;
    bcw->ball = ball;
    bpw->ball = ball;

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {
        if ( bcw->dragging ) {
            bcw->rect.x = mousePos.x + xOffset;
            bcw->rect.y = mousePos.y + yOffset;
        } else if ( bpw->dragging ) {
            bpw->rect.x = mousePos.x + xOffset;
            bpw->rect.y = mousePos.y + yOffset;
        } else if ( gwcw->dragging ) {
            gwcw->rect.x = mousePos.x + xOffset;
            gwcw->rect.y = mousePos.y + yOffset;
        } else if ( ball->dragging ) {
            ball->pos.x = mousePos.x + xOffset;
            ball->pos.y = mousePos.y + yOffset;
        }
    }

    if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
        bcw->dragging = false;
        bpw->dragging = false;
        gwcw->dragging = false;
        for ( int i = 0; i < gw->ballQuantity; i++ ) {
            gw->balls[i]->dragging = false;
        }
        GuiEnable();
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) ) {
        newBall( gw, mousePos );
    }

    for ( int i = 0; i < gw->ballQuantity; i++ ) {
        Ball *b = gw->balls[i];
        if ( b->strokeWidth > b->radius ) {
            b->strokeWidth = b->radius;
        }
    }

    for ( int i = 0; i < gw->ballQuantity; i++ ) {
        Ball *b = gw->balls[i];
        if ( !b->dragging ) {
            moveBall( b, gw );
        } else {
            float xDiff = mousePos.x - prevMousePos.x;
            float yDiff = mousePos.y - prevMousePos.y;
            b->vel.x = xDiff * 50;
            b->vel.y = yDiff * 50;
        }
    }

    prevMousePos = mousePos;

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    if ( gw->drawGrid ) {
        int cellWidth = 50;
        for ( int i = 0; i <= GetScreenHeight() / cellWidth; i++ ) {
            for ( int j = 0; j <= GetScreenWidth() / cellWidth; j++ ) {
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

    for ( int i = 0; i < gw->ballQuantity; i++ ) {
        drawBall( gw->balls[i] );
    }

    drawBallControlWindow( gw->ballControlWindow );
    drawBallPropertyWindow( gw->ballPropertyWindow );
    drawGameWorldControlWindow( gw->controlWindow );
    //DrawFPS( GetScreenWidth() - 100, GetScreenHeight() - 40 );

    EndDrawing();

}

void drawBall( Ball* ball ) {

    ball->strokeColor.a = Lerp( 0, 255, ball->strokeColorAlpha );
    ball->fillColor.a = Lerp( 0, 255, ball->fillColorAlpha );

    DrawCircleSector( ball->pos, ball->radius - ball->strokeWidth, 0, 360, ball->segments, ball->fillColor );
    DrawRing( ball->pos, ball->radius - ball->strokeWidth, ball->radius, 0, 360, ball->segments, ball->strokeColor );

}

void moveBall( Ball* ball, GameWorld *gw ) {
    
    float delta = GetFrameTime();

    ball->pos.x += ball->vel.x * delta;
    ball->pos.y += ball->vel.y * delta;

    if ( ball->pos.x + ball->radius > GetScreenWidth() ) {
        ball->pos.x = GetScreenWidth() - ball->radius;
        ball->vel.x = -ball->vel.x * ball->elasticity;
    } else if ( ball->pos.x - ball->radius < 0 ) {
        ball->pos.x = ball->radius;
        ball->vel.x = -ball->vel.x * ball->elasticity;
    }

    if ( ball->pos.y + ball->radius > GetScreenHeight() ) {
        ball->pos.y = GetScreenHeight() - ball->radius;
        ball->vel.y = -ball->vel.y * ball->elasticity;
    } else if ( ball->pos.y - ball->radius < 0 ) {
        ball->pos.y = ball->radius;
        ball->vel.y = -ball->vel.y * ball->elasticity;
    }

    ball->vel.x = ball->vel.x * ball->friction;
    ball->vel.y = ball->vel.y * ball->friction + gw->gravity;

}

void resetBall( Ball *ball ) {
    ball->pos.x = GetScreenWidth() / 2;
    ball->pos.y = GetScreenHeight() / 2;
    ball->vel.x = 200;
    ball->vel.y = 200;
    ball->friction = 0.99;
    ball->elasticity = 0.9;
    ball->radius = 50;
    ball->segments = 36;
    ball->strokeWidth = 1;
    ball->strokeColor = GetColor( 0x000000ff );
    ball->fillColor = GetColor( 0xffffffff );
    ball->strokeColorAlpha = 1;
    ball->fillColorAlpha = 1;
    ball->dragging = false;
}

void newBall( GameWorld *gw, Vector2 mousePos ) {
    if ( gw->ballQuantity < gw->maxBalls ) {
        Ball *newBall = (Ball*) malloc( sizeof( Ball ) );
        resetBall( newBall );
        newBall->pos.x = mousePos.x;
        newBall->pos.y = mousePos.y;
        newBall->vel.x = GetRandomValue( -400, 400 );
        newBall->vel.y = GetRandomValue( -400, 400 );
        newBall->radius = GetRandomValue( 10, 100 );
        newBall->fillColor.r = GetRandomValue( 100, 255 );
        newBall->fillColor.g = GetRandomValue( 100, 255 );
        newBall->fillColor.b = GetRandomValue( 100, 255 );
        gw->balls[gw->ballQuantity++] = newBall;
    }
}

void resetGameWorld( GameWorld *gw ) {

    gw->gravity = INITIAL_GRAVITY;

    gw->currentBall = gw->balls[0];
    resetBall( gw->currentBall );

    gw->ballControlWindow->ball = gw->currentBall;
    gw->ballPropertyWindow->ball = gw->currentBall;

    for ( int i = 1; i < gw->ballQuantity; i++ ) {
        free( gw->balls[i] );
    }

    gw->ballQuantity = 1;

}

void drawBallControlWindow( BallControlWindow *bcw ) {

    if ( bcw->visible ) {

        Ball *ball = bcw->ball;

        if ( GuiWindowBox( bcw->rect, "Controle da Bolinha" ) == 1 ) {
            bcw->visible = false;
        }

        GuiSlider( (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop, 200, 20 }, "raio:", TextFormat( "%2.2f", ball->radius ), &(ball->radius), 10, 300 );
        GuiSlider( (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 25, 200, 20 }, "segmentos:", TextFormat( "%2.2f", ball->segments ), &(ball->segments), 4, 100 );
        GuiSlider( (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 50, 200, 20 }, "contorno:", TextFormat( "%2.2f", ball->strokeWidth ), &(ball->strokeWidth), 0, bcw->ball->radius );

        Rectangle rStrokeColor = (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 75, 200, 200 };
        Rectangle rStrokeAlpha = (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 280, 200, 20 };
        GuiColorPicker( rStrokeColor, NULL, &(ball->strokeColor) );
        GuiColorBarAlpha( rStrokeAlpha, "transparência", &(ball->strokeColorAlpha) );

        Rectangle rFillColor = (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 305, 200, 200 };
        Rectangle rFillAlpha = (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 510, 200, 20 };
        GuiColorPicker( rFillColor, NULL, &(ball->fillColor) );
        GuiColorBarAlpha( rFillAlpha, "transparência", &(ball->fillColorAlpha) );

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

        GuiSlider( (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 535, 200, 20 }, "atrito:", TextFormat( "%2.2f", ball->friction ), &(ball->friction), 0.9, 1 );
        GuiSlider( (Rectangle){ bcw->rect.x + bcw->marginLeft, bcw->rect.y + bcw->marginTop + 560, 200, 20 }, "elasticidade:", TextFormat( "%2.2f", ball->elasticity ), &(ball->elasticity), 0, 1 );

    }

}

void drawBallPropertyWindow( BallPropertyWindow *bpw ) {

    if ( bpw->visible ) {

        Ball *ball = bpw->ball;

        if ( GuiWindowBox( bpw->rect, "Propriedades da Bolinha" ) == 1 ) {
            bpw->visible = false;
        }

        Rectangle rGroup = { bpw->rect.x + bpw->marginLeft, bpw->rect.y + bpw->marginTop + 5, 100, 90 };
        GuiGroupBox( rGroup, "Propriedades" );
        GuiLabel( (Rectangle){ rGroup.x + 10, rGroup.y + 5, 100, 20 }, TextFormat( "x: %2.2f", ball->pos.x ) );
        GuiLabel( (Rectangle){ rGroup.x + 10, rGroup.y + 25, 100, 20 }, TextFormat( "y: %2.2f", ball->pos.y ) );
        GuiLabel( (Rectangle){ rGroup.x + 10, rGroup.y + 45, 100, 20 }, TextFormat( "vel. x: %2.2f", ball->vel.x ) );
        GuiLabel( (Rectangle){ rGroup.x + 10, rGroup.y + 65, 100, 20 }, TextFormat( "vel. y: %2.2f", ball->vel.y ) );

    }

}
void drawGameWorldControlWindow( GameWorldControlWindow *gwcw ) {

    if ( gwcw->visible ) {

        GameWorld *gw = gwcw->gw;

        if ( GuiWindowBox( gwcw->rect, "Controles Gerais" ) == 1 ) {
            gwcw->visible = false;
        }

        GuiSlider( (Rectangle){ gwcw->rect.x + gwcw->marginLeft, gwcw->rect.y + gwcw->marginTop, 200, 20 }, "gravidade:", TextFormat( "%2.2f", gw->gravity ), &(gw->gravity), 0, 200 );
        GuiCheckBox( (Rectangle){ gwcw->rect.x + gwcw->marginLeft, gwcw->rect.y + gwcw->marginTop + 25, 20, 20 }, "grade", &(gw->drawGrid) );

    }

}

bool checkCollisionBallMouse( Ball *ball, Vector2 mousePos ) {
    return CheckCollisionPointCircle( mousePos, ball->pos, ball->radius );
}

bool checkCollisionBallControlWindowMouse( BallControlWindow *bcw, Vector2 mousePos ) {
    Rectangle titleBarRect = {
        .x = bcw->rect.x,
        .y = bcw->rect.y,
        .width = bcw->rect.width,
        .height = bcw->titleBarHeight
    };
    return CheckCollisionPointRec( mousePos, titleBarRect );
}

bool checkCollisionBallPropertyWindowMouse( BallPropertyWindow *bpw, Vector2 mousePos ) {
    Rectangle titleBarRect = {
        .x = bpw->rect.x,
        .y = bpw->rect.y,
        .width = bpw->rect.width,
        .height = bpw->titleBarHeight
    };
    return CheckCollisionPointRec( mousePos, titleBarRect );
}

bool checkCollisionGameWorldControlWindowMouse( GameWorldControlWindow *gwcw, Vector2 mousePos ) {
    Rectangle titleBarRect = {
        .x = gwcw->rect.x,
        .y = gwcw->rect.y,
        .width = gwcw->rect.width,
        .height = gwcw->titleBarHeight
    };
    return CheckCollisionPointRec( mousePos, titleBarRect );
}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    resetBall( &ball );

    bcw = (BallControlWindow){
        .rect = {
            .x = 20,
            .y = 20,
            .width = 350,
            .height = 620
        },
        .marginTop = 30,
        .marginLeft = 110,
        .titleBarHeight = 25,
        .visible = true,
        .dragging = false,
        .ball = &ball
    };

    bpw = (BallPropertyWindow){
        .rect = {
            .x = bcw.rect.x + bcw.rect.width + 5,
            .y = bcw.rect.y,
            .width = 160,
            .height = 135
        },
        .marginTop = 30,
        .marginLeft = 10,
        .titleBarHeight = 25,
        .visible = true,
        .dragging = false,
        .ball = &ball
    };

    gwcw = (GameWorldControlWindow){
        .rect = {
            .x = bcw.rect.x + bcw.rect.width + 5,
            .y = bpw.rect.y + bpw.rect.height + 5,
            .width = 300,
            .height = 85
        },
        .marginTop = 30,
        .marginLeft = 65,
        .titleBarHeight = 25,
        .visible = true,
        .dragging = false,
        .gw = &gw
    };

    gw = (GameWorld) {
        .currentBall = &ball,
        .balls[0] = &ball,
        .ballQuantity = 1,
        .maxBalls = MAX_BALLS,
        .ballControlWindow = &bcw,
        .ballPropertyWindow = &bpw,
        .controlWindow = &gwcw,
        .gravity = INITIAL_GRAVITY,
        .drawGrid = true
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