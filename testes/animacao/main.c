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
//#include <raymath.h>

//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>
//#undef RAYGUI_IMPLEMENTATION

/*---------------------------------------------
 * Project headers.
 --------------------------------------------*/
#include <utils.h>

/*---------------------------------------------
 * Macros. 
 --------------------------------------------*/
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Window Title"
#define WINDOW_TARGET_FPS 60
#define WINDOW_ANTIALIASING
//#define WINDOW_RESIZABLE
//#define WINDOW_FULL_SCREEN
//#define WINDOW_UNDECORATED
//#define WINDOW_ALWAYS_ON_TOP
//#define WINDOW_ALWAYS_RUN
//#define WINDOW_INIT_AUDIO

/*--------------------------------------------
 * Constants. 
 -------------------------------------------*/


/*---------------------------------------------
 * Custom types (enums, structs, unions etc.).
 --------------------------------------------*/
typedef enum EstadoMario {
    ESTADO_MARIO_PARADO,
    ESTADO_MARIO_ANDANDO
} EstadoMario;

typedef enum Direcao {
    DIRECAO_DIREITA,
    DIRECAO_ESQUERDA,
} Direcao;

typedef struct Animacao {
    int quadroAtual;
    int maximoQuadros;
    float temporizador;
    float tempoParaMudarQuadro;
    Texture2D *texturas;
} Animacao;

typedef struct Mario {
    Vector2 pos;
    Vector2 vel;
    Animacao animacaoParadoDireita;
    Animacao animacaoParadoEsquerda;
    Animacao animacaoPequenoDireita;
    Animacao animacaoPequenoEsquerda;
    EstadoMario estado;
    Direcao olhandoPara;
} Mario;

typedef struct GameWorld {
    Mario *mario;
} GameWorld;


/*---------------------------------------------
 * Global variables.
 --------------------------------------------*/
GameWorld gw;
Mario mario;

Texture2D texturaQuadro1;
Texture2D texturaQuadro2;


/*---------------------------------------------
 * Function prototypes. 
 --------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 * @param gw GameWorld struct pointer.
 */
void inputAndUpdate( GameWorld *gw );
void inputAndUpdateMario( Mario *mario );
void updateAnimacao( Animacao *animacao );

/**
 * @brief Draws the state of the game.
 * @param gw GameWorld struct pointer.
 */
void draw( GameWorld *gw );
void drawMario( Mario *mario );
void drawAnimacao( Animacao *animacao, Vector2 *pos );

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

Texture2D flipTexture2DHorizontal( Texture2D *texture );

int main( void ) {

    #ifdef WINDOW_ANTIALIASING
        SetConfigFlags( FLAG_MSAA_4X_HINT );
    #endif

    #ifdef WINDOW_RESIZABLE
        SetConfigFlags( FLAG_WINDOW_RESIZABLE );
    #endif

    #ifdef WINDOW_FULL_SCREEN
        SetConfigFlags( FLAG_FULLSCREEN_MODE );
    #endif

    #ifdef WINDOW_UNDECORATED
        SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    #endif

    #ifdef WINDOW_ALWAYS_ON_TOP
        SetConfigFlags( FLAG_WINDOW_TOPMOST );
    #endif

    #ifdef WINDOW_ALWAYS_RUN
        SetConfigFlags( FLAG_WINDOW_ALWAYS_RUN );
    #endif

    InitWindow( WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE );
    #ifdef WINDOW_INIT_AUDIO
        InitAudioDevice();
    #endif
    SetTargetFPS( WINDOW_TARGET_FPS );    

    loadResources();
    createGameWorld();
    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }
    unloadResources();
    destroyGameWorld();

    #ifdef WINDOW_INIT_AUDIO
        CloseAudioDevice();
    #endif
    CloseWindow();
    return 0;

}

void inputAndUpdate( GameWorld *gw ) {
    inputAndUpdateMario( gw->mario );
}

void inputAndUpdateMario( Mario *mario ) {

    float delta = GetFrameTime();

    if ( IsKeyDown( KEY_RIGHT ) ) {
        mario->estado = ESTADO_MARIO_ANDANDO;
        mario->olhandoPara = DIRECAO_DIREITA;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        mario->estado = ESTADO_MARIO_ANDANDO;
        mario->olhandoPara = DIRECAO_ESQUERDA;
    } else {
        mario->estado = ESTADO_MARIO_PARADO;
    }

    if ( mario->estado == ESTADO_MARIO_ANDANDO ) {
        mario->pos.x += ( mario->olhandoPara == DIRECAO_DIREITA ? mario->vel.x : -mario->vel.x ) * delta;
        mario->pos.y += mario->vel.y * delta;
    }

    switch ( mario->estado ) {
        case ESTADO_MARIO_PARADO: // não precisa...
            break;
        case ESTADO_MARIO_ANDANDO:
            if ( mario->olhandoPara == DIRECAO_DIREITA ) updateAnimacao( &mario->animacaoPequenoDireita );
            else if ( mario->olhandoPara == DIRECAO_ESQUERDA ) updateAnimacao( &mario->animacaoPequenoEsquerda );
            break;
        default:
            break;
    }
}

void updateAnimacao( Animacao *animacao ) {

    if ( animacao->maximoQuadros != 1 ) {
        animacao->temporizador += GetFrameTime();
        if ( animacao->temporizador >= animacao->tempoParaMudarQuadro ) {
            animacao->temporizador = 0;
            animacao->quadroAtual++;
            animacao->quadroAtual %= animacao->maximoQuadros;
        }
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawMario( gw->mario );

    EndDrawing();

}

void drawMario( Mario *mario ) {
    if ( mario->olhandoPara == DIRECAO_DIREITA ) {

    }
    switch ( mario->estado ) {
        case ESTADO_MARIO_PARADO:
            if ( mario->olhandoPara == DIRECAO_DIREITA ) drawAnimacao( &mario->animacaoParadoDireita, &mario->pos );
            else if ( mario->olhandoPara == DIRECAO_ESQUERDA ) drawAnimacao( &mario->animacaoParadoEsquerda, &mario->pos );
            break;
        case ESTADO_MARIO_ANDANDO:
            if ( mario->olhandoPara == DIRECAO_DIREITA ) drawAnimacao( &mario->animacaoPequenoDireita, &mario->pos );
            else if ( mario->olhandoPara == DIRECAO_ESQUERDA ) drawAnimacao( &mario->animacaoPequenoEsquerda, &mario->pos );
            break;
        default:
            break;
    }
    
}

void drawAnimacao( Animacao *animacao, Vector2 *pos ) {
    DrawTexture( animacao->texturas[animacao->quadroAtual], pos->x, pos->y, WHITE );
}

void createGameWorld( void ) {

    printf( "creating game world...\n" );

    mario = (Mario) {
        .pos = {
            .x = 100,
            .y = 100
        },
        .vel = {
            .x = 200,
            .y = 0
        },
        .animacaoParadoDireita = {
            .quadroAtual = 0,
            .maximoQuadros = 1
        },
        .animacaoParadoEsquerda = {
            .quadroAtual = 0,
            .maximoQuadros = 1
        },
        .animacaoPequenoDireita = {
            .quadroAtual = 0,
            .maximoQuadros = 2,
            .temporizador = 0,
            .tempoParaMudarQuadro = 0.2
        },
        .animacaoPequenoEsquerda = {
            .quadroAtual = 0,
            .maximoQuadros = 2,
            .temporizador = 0,
            .tempoParaMudarQuadro = 0.2
        },
        .estado = ESTADO_MARIO_PARADO
    };

    mario.animacaoParadoDireita.texturas = (Texture2D*) malloc( mario.animacaoParadoDireita.maximoQuadros * sizeof(Texture2D) );
    mario.animacaoParadoDireita.texturas[0] = texturaQuadro1;
    mario.animacaoParadoEsquerda.texturas = (Texture2D*) malloc( mario.animacaoParadoEsquerda.maximoQuadros * sizeof(Texture2D) );
    mario.animacaoParadoEsquerda.texturas[0] = flipTexture2DHorizontal( &texturaQuadro1 );

    mario.animacaoPequenoDireita.texturas = (Texture2D*) malloc( mario.animacaoPequenoDireita.maximoQuadros * sizeof(Texture2D) );
    mario.animacaoPequenoDireita.texturas[0] = texturaQuadro1;
    mario.animacaoPequenoDireita.texturas[1] = texturaQuadro2;
    mario.animacaoPequenoEsquerda.texturas = (Texture2D*) malloc( mario.animacaoPequenoEsquerda.maximoQuadros * sizeof(Texture2D) );
    mario.animacaoPequenoEsquerda.texturas[0] = flipTexture2DHorizontal( &texturaQuadro1 );
    mario.animacaoPequenoEsquerda.texturas[1] = flipTexture2DHorizontal( &texturaQuadro2 );

    gw = (GameWorld) {
        .mario = &mario
    };

}

void destroyGameWorld( void ) {
    printf( "destroying game world...\n" );
    free( mario.animacaoParadoDireita.texturas );
    free( mario.animacaoParadoEsquerda.texturas );
    free( mario.animacaoPequenoDireita.texturas );
    free( mario.animacaoPequenoEsquerda.texturas );
}

void loadResources( void ) {
    printf( "loading resources...\n" );
    texturaQuadro1 = LoadTexture( "resources/SmallMario_0.png" );
    texturaQuadro2 = LoadTexture( "resources/SmallMario_1.png" );
}

void unloadResources( void ) {
    printf( "unloading resources...\n" );
    UnloadTexture( texturaQuadro1 );
    UnloadTexture( texturaQuadro2 );
}

Texture2D flipTexture2DHorizontal( Texture2D *texture ) {

    Image img = LoadImageFromTexture( *texture );
    ImageFlipHorizontal( &img );
    Texture2D tex = LoadTextureFromImage( img );
    UnloadImage( img );

    return tex;

}