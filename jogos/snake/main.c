/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Base template for game
 * development in C using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/*
 * TODO LIST:
 *     - alimentar e contar pontos;
 *           - ao alimentar, aumentar a cobra em uma peça quando
 *             a comida chegar ao fim
 *     - tempo de vida nas comidas
 *     - ajustar frames por segundo (aumentar fps e contar tempo de movimentação)
 *     - aumentar velocidade da cobra quando chegar a determinados pontos
 *       análogo à ideia de níveis
 *     - finalizar jogo ganhando (não sobrar espaço no tabuleiro)
 */

/*---------------------------------------------
 * Library headers.
 --------------------------------------------*/
#include "include/raylib.h"

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include "include/utils.h"

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/
#define BOARD_LINES 18
#define BOARD_COLUMNS 20
#define BOARD_MAX_SNAKE_FOOD 1
#define SNAKE_FOOD_QUANTITY 5


/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const int BOARD_CELL_WIDTH = 30;
const Color BOARD_BORDER_COLOR = { 144, 155, 117, 255 };
const Color BOARD_CENTER_COLOR = { 157, 171, 133, 255 };
const Color BOARD_CELL_COLOR = { 0, 0, 0, 20 };
const int BOARD_MARGIN = 10;

const Color SNAKE_BODY_COLOR = { 11, 8, 11, 255 };
const Color SNAKE_FOOD_COLORS[SNAKE_FOOD_QUANTITY] = {
    { 133, 53, 48, 255 },
    { 181, 86, 0, 255 },
    { 67, 138, 43, 255 },
    { 39, 70, 141, 255 },
    { 120, 41, 139, 255 }
};
const int SNAKE_FOOD_POINTS[SNAKE_FOOD_QUANTITY] = { 5, 10, 15, 20, 25 };

const Color OVERLAY_COLOR = { 0, 0, 0, 150 };

const int SCORE_HEIGHT = 40;
const int SCORE_MARGIN = 10;


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
typedef enum GameState {
    GAME_STATE_IDLE,
    GAME_STATE_PLAYING,
    GAME_STATE_WON,
    GAME_STATE_LOSE
} GameState;

typedef enum SnakeDirection {
    SNAKE_GOING_TO_RIGHT,
    SNAKE_GOING_TO_LEFT,
    SNAKE_GOING_TO_UP,
    SNAKE_GOING_TO_DOWN,
} SnakeDirection;

typedef struct Snake {
    int cellWidth;
    Vector2 body[BOARD_LINES*BOARD_COLUMNS];
    int bodyPieces;
    SnakeDirection direction;
    Color color;
} Snake;

typedef struct SnakeFood {
    int line;
    int column;
    int cellWidth;
    Color color;
    int points;
} SnakeFood;

typedef struct Score {
    Vector2 pos;
    int width;
    int height;
    int margin;
    int points;
} Score;

typedef struct GameWorld {
    int lines;
    int columns;
    int cellWidth;
    int margin;
    Color borderColor;
    Color centerColor;
    Color cellColor;
    Snake *snake;
    SnakeFood snakeFood[BOARD_MAX_SNAKE_FOOD];
    int snakeFoodQuantity;
    Score *score;
    GameState state;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Snake snake;
SnakeFood snakeFood;
Score score;


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
void draw( const GameWorld *gw );

void drawSnake( const Snake *snake, int hOffset, int vOffset );
void drawSnakeFood( const SnakeFood *snakeFood, int hOffset, int vOffset );
void drawScore( const Score *score );
SnakeFood newSnakeFood( void );

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

    const int screenWidth = BOARD_COLUMNS * BOARD_CELL_WIDTH + BOARD_MARGIN * 2;
    const int screenHeight = BOARD_LINES * BOARD_CELL_WIDTH + SCORE_HEIGHT + BOARD_MARGIN * 2;

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Snake" );
    InitAudioDevice();
    SetTargetFPS( 10 );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    destroyGameWorld();
    unloadResources();

    CloseAudioDevice();
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    Snake *snake = gw->snake;
    Vector2 temp = {0};
    Vector2 previous = {0};

    if ( gw->state == GAME_STATE_IDLE ) {
        if ( GetKeyPressed() != 0 ) {
            gw->state = GAME_STATE_PLAYING;
        }
    } else if ( gw->state == GAME_STATE_WON || gw->state == GAME_STATE_LOSE ) {
        if ( GetKeyPressed() != 0 ) {
            destroyGameWorld();
            createGameWorld();
            gw->state = GAME_STATE_PLAYING;
        }
    } else if ( gw->state == GAME_STATE_PLAYING ) {

        if ( IsKeyPressed( KEY_LEFT ) ) {
            if ( snake->direction != SNAKE_GOING_TO_RIGHT ) {
                snake->direction = SNAKE_GOING_TO_LEFT;
            }
        } else if ( IsKeyPressed( KEY_RIGHT ) ) {
            if ( snake->direction != SNAKE_GOING_TO_LEFT ) {
                snake->direction = SNAKE_GOING_TO_RIGHT;
            }
        } else if ( IsKeyPressed( KEY_UP ) ) {
            if ( snake->direction != SNAKE_GOING_TO_DOWN ) {
                snake->direction = SNAKE_GOING_TO_UP;
            }
        } else if ( IsKeyPressed( KEY_DOWN ) ) {
            if ( snake->direction != SNAKE_GOING_TO_UP ) {
                snake->direction = SNAKE_GOING_TO_DOWN;
            }
        }

        for ( int i = 0; i < snake->bodyPieces; i++ ) {
            Vector2 *current = &snake->body[i];
            Vector2 temp = *current;
            if ( i == 0 ) {
                switch ( snake->direction ) {
                    case SNAKE_GOING_TO_LEFT:
                        current->x--;
                        break;
                    case SNAKE_GOING_TO_RIGHT:
                        current->x++;
                        break;
                    case SNAKE_GOING_TO_UP:
                        current->y--;
                        break;
                    case SNAKE_GOING_TO_DOWN:
                        current->y++;
                        break;
                }
                if ( current->x < 0 ) {
                    current->x = gw->columns-1;
                }
                if ( current->y < 0 ) {
                    current->y = gw->lines-1;
                }
                current->x = ( (int) current->x ) % gw->columns;
                current->y = ( (int) current->y ) % gw->lines;
            } else {
                current->x = previous.x;
                current->y = previous.y;
            }
            previous = temp;
        }

        // bite itself
        Vector2 *head = &snake->body[0];
        for ( int i = 1; i < snake->bodyPieces; i++ ) {
            if ( head->x == snake->body[i].x &&
                 head->y == snake->body[i].y ) {
                gw->state = GAME_STATE_LOSE;
            }
        }

    }


}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    int hOffset = gw->margin;
    int vOffset = gw->score->height + gw->margin;

    DrawRectangleGradientH( 
        0, 0, 
        GetScreenWidth() / 2, GetScreenHeight(), 
        gw->borderColor, gw->centerColor );
    DrawRectangleGradientH( 
        GetScreenWidth() / 2, 0, 
        GetScreenWidth() / 2, GetScreenHeight(), 
        gw->centerColor, gw->borderColor );

    DrawRectangleLines( 
        gw->margin/2, 
        gw->margin/2 + gw->score->height, 
        GetScreenWidth() - gw->margin, 
        GetScreenHeight() - vOffset, BLACK );

    for ( int i = 0; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            DrawRectangleLinesEx( 
                (Rectangle){
                    .x = j * gw->cellWidth + 2 + hOffset,
                    .y = i * gw->cellWidth + 2 + vOffset,
                    .width = gw->cellWidth - 2,
                    .height = gw->cellWidth - 2
                },
                2,
                gw->cellColor
            );
            DrawRectangle(  
                j * gw->cellWidth + 6 + hOffset,
                i * gw->cellWidth + 6 + vOffset,
                gw->cellWidth - 10,
                gw->cellWidth - 10,
                gw->cellColor
            );
        }
    }

    drawSnake( gw->snake, hOffset, vOffset );
    for ( int i = 0; i < gw->snakeFoodQuantity; i++ ) {
        drawSnakeFood( &gw->snakeFood[i], hOffset, vOffset );
    }
    drawScore( gw->score );

    if ( gw->state != GAME_STATE_PLAYING ) {
        DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), OVERLAY_COLOR );
    }

    if ( gw->state == GAME_STATE_IDLE ) {
        const char *m = "tecle algo para começar.";
        int w = MeasureText( m, 20 );
        DrawText( m, GetScreenWidth() - w - 10, vOffset + 2, 20, BLACK );
        DrawText( m, GetScreenWidth() - w - 12, vOffset, 20, WHITE );
    } else if ( gw->state == GAME_STATE_WON ) {

        const char *m1 = "você ganhou!";
        int w = MeasureText( m1, 20 );
        DrawText( m1, GetScreenWidth() - w - 10, vOffset + 2, 20, BLACK );
        DrawText( m1, GetScreenWidth() - w - 12, vOffset, 20, LIME );

        const char *m2 = "tecle algo para recomeçar.";
        w = MeasureText( m2, 20 );
        DrawText( m2, GetScreenWidth() - w - 10, vOffset + 22, 20, BLACK );
        DrawText( m2, GetScreenWidth() - w - 12, vOffset + 20, 20, WHITE );

    } else if ( gw->state == GAME_STATE_LOSE ) {
        
        const char *m1 = "você perdeu...";
        int w = MeasureText( m1, 20 );
        DrawText( m1, GetScreenWidth() - w - 10, vOffset + 2, 20, BLACK );
        DrawText( m1, GetScreenWidth() - w - 12, vOffset, 20, MAROON );

        const char *m2 = "tecle algo para recomeçar.";
        w = MeasureText( m2, 20 );
        DrawText( m2, GetScreenWidth() - w - 10, vOffset + 22, 20, BLACK );
        DrawText( m2, GetScreenWidth() - w - 12, vOffset + 20, 20, WHITE );

    }

    EndDrawing();

}

void drawSnake( const Snake *snake, int hOffset, int vOffset ) {

    for ( int i = snake->bodyPieces-1; i >=0; i-- ) {
        
        const Vector2 *c = &snake->body[i];

        DrawRectangleLinesEx( 
            (Rectangle){
                .x = c->x * snake->cellWidth + 2 + hOffset,
                .y = c->y * snake->cellWidth + 2 + vOffset,
                .width = snake->cellWidth - 2,
                .height = snake->cellWidth - 2
            },
            2,
            snake->color
        );

        DrawRectangle(  
            c->x * snake->cellWidth + 6 + hOffset,
            c->y * snake->cellWidth + 6 + vOffset,
            snake->cellWidth - 10,
            snake->cellWidth - 10,
            snake->color
        );

    }

}

void drawSnakeFood( const SnakeFood *snakeFood, int hOffset, int vOffset ) {

    DrawRectangleLinesEx( 
        (Rectangle){
            .x = snakeFood->column * snakeFood->cellWidth + 2 + hOffset,
            .y = snakeFood->line * snakeFood->cellWidth + 2 + vOffset,
            .width = snakeFood->cellWidth - 2,
            .height = snakeFood->cellWidth - 2
        },
        2,
        snakeFood->color
    );

    DrawRectangle(  
        snakeFood->column * snakeFood->cellWidth + 6 + hOffset,
        snakeFood->line * snakeFood->cellWidth + 6 + vOffset,
        snakeFood->cellWidth - 10,
        snakeFood->cellWidth - 10,
        snakeFood->color
    );

}

void drawScore( const Score *score ) {
    const char *t = TextFormat( "%05d", score->points );
    DrawText( t, score->pos.x + score->margin, score->pos.y + score->margin, 30, BLACK );
}

SnakeFood newSnakeFood( void ) {

    int p = GetRandomValue( 0, SNAKE_FOOD_QUANTITY-1 );

    return (SnakeFood) {
        .line = GetRandomValue( 0, BOARD_LINES-1 ),
        .column = GetRandomValue( 0, BOARD_COLUMNS-1 ),
        .cellWidth = BOARD_CELL_WIDTH,
        .color = SNAKE_FOOD_COLORS[p],
        .points = SNAKE_FOOD_POINTS[p],
    };

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    snake = (Snake) {
        .cellWidth = BOARD_CELL_WIDTH,
        .body = {0},
        .bodyPieces = 0,
        .direction = SNAKE_GOING_TO_RIGHT,
        .color = SNAKE_BODY_COLOR
    };

    for ( int i = 0; i < 8; i++ ) {
        snake.body[snake.bodyPieces++] = (Vector2){ 
            .x = BOARD_COLUMNS / 2 - i, 
            .y = BOARD_LINES / 2 - 1
        };
    }

    score = (Score) {
        .pos = {
            .x = 0,
            .y = 0
        },
        .width = GetScreenWidth(),
        .height = SCORE_HEIGHT,
        .margin = SCORE_MARGIN,
        .points = 0
    };

    gw = (GameWorld) {
        .lines = BOARD_LINES,
        .columns = BOARD_COLUMNS,
        .cellWidth = BOARD_CELL_WIDTH,
        .margin = BOARD_MARGIN,
        .borderColor = BOARD_BORDER_COLOR,
        .centerColor = BOARD_CENTER_COLOR,
        .cellColor = BOARD_CELL_COLOR,
        .snake = &snake,
        .snakeFood = {0},
        .snakeFoodQuantity = 0,
        .score = &score,
        .state = GAME_STATE_IDLE
    };

    for ( int i = 0; i < BOARD_MAX_SNAKE_FOOD; i++ ) {
        gw.snakeFood[gw.snakeFoodQuantity++] = newSnakeFood();
    }

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