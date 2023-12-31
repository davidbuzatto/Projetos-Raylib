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

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/
const int BOARD_CELL_WIDTH = 30;
const Color BOARD_BORDER_COLOR = { 144, 155, 117, 255 };
const Color BOARD_CENTER_COLOR = { 157, 171, 133, 255 };
const Color BOARD_CELL_COLOR = { 0, 0, 0, 20 };
const Color SNAKE_BODY_COLOR = { 11, 8, 11, 255 };
const Color SNAKE_FOOD_COLOR = { 133, 53, 48, 255 };

/*---------------------------------------------
 * Custom types (enums, structs, unions etc.)
 --------------------------------------------*/
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
} SnakeFood;

typedef struct GameWorld {
    int lines;
    int columns;
    int cellWidth;
    Snake *snake;
    SnakeFood *snakeFood;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Snake snake;
SnakeFood snakeFood;


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

void drawSnake( const Snake *snake );
void drawSnakeFood( const SnakeFood *snakeFood );

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

    const int screenWidth = BOARD_COLUMNS * BOARD_CELL_WIDTH;
    const int screenHeight = BOARD_LINES * BOARD_CELL_WIDTH;

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

    if ( IsKeyPressed( KEY_LEFT ) ) {
        snake->direction = SNAKE_GOING_TO_LEFT;
    } else if ( IsKeyPressed( KEY_RIGHT ) ) {
        snake->direction = SNAKE_GOING_TO_RIGHT;
    } else if ( IsKeyPressed( KEY_UP ) ) {
        snake->direction = SNAKE_GOING_TO_UP;
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        snake->direction = SNAKE_GOING_TO_DOWN;
    }

    for ( int i = 0; i < snake->bodyPieces; i++ ) {
        Vector2 temp = snake->body[i];
        if ( i == 0 ) {
            switch ( snake->direction ) {
                case SNAKE_GOING_TO_LEFT:
                    snake->body[i].x--;
                    break;
                case SNAKE_GOING_TO_RIGHT:
                    snake->body[i].x++;
                    break;
                case SNAKE_GOING_TO_UP:
                    snake->body[i].y--;
                    break;
                case SNAKE_GOING_TO_DOWN:
                    snake->body[i].y++;
                    break;
            }
        } else {
            snake->body[i].x = previous.x;
            snake->body[i].y = previous.y;
        }
        previous = temp;
    }

    

}

void draw( const GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    DrawRectangleGradientH( 
        0, 0, 
        GetScreenWidth() / 2, GetScreenHeight(), 
        BOARD_BORDER_COLOR, BOARD_CENTER_COLOR );
    DrawRectangleGradientH( 
        GetScreenWidth() / 2, 0, 
        GetScreenWidth() / 2, GetScreenHeight(), 
        BOARD_CENTER_COLOR, BOARD_BORDER_COLOR );

    for ( int i = 0; i < gw->lines; i++ ) {
        for ( int j = 0; j < gw->columns; j++ ) {
            DrawRectangleLinesEx( 
                (Rectangle){
                    .x = j * gw->cellWidth + 2,
                    .y = i * gw->cellWidth + 2,
                    .width = gw->cellWidth - 2,
                    .height = gw->cellWidth - 2
                },
                2,
                BOARD_CELL_COLOR
            );
            DrawRectangle(  
                j * gw->cellWidth + 6,
                i * gw->cellWidth + 6,
                gw->cellWidth - 10,
                gw->cellWidth - 10,
                BOARD_CELL_COLOR
            );
        }
    }

    drawSnake( gw->snake );
    drawSnakeFood( gw->snakeFood );

    EndDrawing();

}

void drawSnake( const Snake *snake ) {

    for ( int i = 0; i < snake->bodyPieces; i++ ) {
        
        const Vector2 *c = &snake->body[i];

        DrawRectangleLinesEx( 
            (Rectangle){
                .x = c->x * snake->cellWidth + 2,
                .y = c->y * snake->cellWidth + 2,
                .width = snake->cellWidth - 2,
                .height = snake->cellWidth - 2
            },
            2,
            snake->color
        );

        DrawRectangle(  
            c->x * snake->cellWidth + 6,
            c->y * snake->cellWidth + 6,
            snake->cellWidth - 10,
            snake->cellWidth - 10,
            snake->color
        );

    }

}

void drawSnakeFood( const SnakeFood *snakeFood ) {

    DrawRectangleLinesEx( 
        (Rectangle){
            .x = snakeFood->column * snakeFood->cellWidth + 2,
            .y = snakeFood->line * snakeFood->cellWidth + 2,
            .width = snakeFood->cellWidth - 2,
            .height = snakeFood->cellWidth - 2
        },
        2,
        snakeFood->color
    );

    DrawRectangle(  
        snakeFood->column * snakeFood->cellWidth + 6,
        snakeFood->line * snakeFood->cellWidth + 6,
        snakeFood->cellWidth - 10,
        snakeFood->cellWidth - 10,
        snakeFood->color
    );

}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    snake = (Snake) {
        .cellWidth = BOARD_CELL_WIDTH,
        .body = { 
            (Vector2){ 
                .x = 2, 
                .y = 8 
            },
            (Vector2){ 
                .x = 2, 
                .y = 7 
            },
            (Vector2){ 
                .x = 2, 
                .y = 6 
            },
            (Vector2){ 
                .x = 2, 
                .y = 5
            },
            (Vector2){ 
                .x = 2, 
                .y = 4 
            },
            (Vector2){ 
                .x = 2, 
                .y = 3 
            },
            (Vector2){ 
                .x = 2, 
                .y = 2 
            },
            (Vector2){ 
                .x = 2, 
                .y = 1
            }
        },
        .bodyPieces = 8,
        .direction = SNAKE_GOING_TO_DOWN,
        .color = SNAKE_BODY_COLOR
    };

    snakeFood = (SnakeFood) {
        .line = 9,
        .column = 5,
        .cellWidth = BOARD_CELL_WIDTH,
        .color = SNAKE_FOOD_COLOR
    };

    gw = (GameWorld) {
        .lines = BOARD_LINES,
        .columns = BOARD_COLUMNS,
        .cellWidth = BOARD_CELL_WIDTH,
        .snake = &snake,
        .snakeFood = &snakeFood
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