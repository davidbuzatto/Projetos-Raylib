/*******************************************************************************************
* Teste Bolinha
* Autor: Prof. Dr. David Buzatto
********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "include/raylib.h"

typedef struct {
    double x;
    double y;
    double raio;
    double vx;
    double vy;
    double atrito;
    double elasticidade;
    bool emArraste;
    Color cor;
} Bolinha;

#define MAX_B 100

void criarBolinha( double x, double y, double raio, double vx, double vy, double atrito, double elasticidade, int r, int g, int b );
void desenharBolinha( Bolinha *bolinha );
void desenharBolinhas();
void atualizarBolinhas();
void processarMouse();

double gravidade = 5;
bool mouseArrastando = false;

double xPress;
double yPress;

double currX;
double currY;
double lastX;
double lastY;

double offsetX;
double offsetY;

Bolinha *bolinhas[MAX_B];
int quantidadeBolinhas = 0;

int main() {

    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Bolinha - Prof. Dr. David Buzatto" );
    SetTargetFPS( 60 );

    criarBolinha( 100, 100, 25, 2, 2, 0.99, 0.9, 0, 0, 255 );

    while ( !WindowShouldClose() ) {

        // atualizar
        processarMouse();
        atualizarBolinhas();
        
        // desenhar
        BeginDrawing();
        ClearBackground( RAYWHITE );
        desenharBolinhas();
        EndDrawing();

    }

    // fecha a janela e o contexto OpenGL
    CloseWindow();
    return 0;

}

void criarBolinha( double x, double y, 
                   double raio, 
                   double vx, double vy, 
                   double atrito, double elasticidade,
                   int r, int g, int b ) {

    if ( quantidadeBolinhas < MAX_B ) {

        Bolinha *nova = (Bolinha*) malloc( sizeof(Bolinha) );
        
        nova->x = x;
        nova->y = y;
        nova->raio = raio;
        nova->vx = vx;
        nova->vx = vy;
        nova->atrito = atrito;
        nova->elasticidade = elasticidade;
        nova->emArraste = false;
        nova->cor.r = r;
        nova->cor.g = g;
        nova->cor.b = b;
        nova->cor.a = 255;

        bolinhas[quantidadeBolinhas++] = nova;

    }
    
}

void desenharBolinha( Bolinha *bolinha ) {
    DrawCircle( (int) bolinha->x, (int) bolinha->y, (int) bolinha->raio, bolinha->cor );
    DrawCircleLines( (int) bolinha->x, (int) bolinha->y, (int) bolinha->raio, BLACK );
}

void desenharBolinhas() {
    for ( int i = 0; i < quantidadeBolinhas; i++ ) {
        desenharBolinha( bolinhas[i] );
    }
}

void atualizarBolinhas() {

    for ( int i = 0; i < quantidadeBolinhas; i++ ) {

        Bolinha *bolinha = bolinhas[i];

        if ( !bolinha->emArraste ) {

            bolinha->x += bolinha->vx;
            bolinha->y += bolinha->vy;

            if ( bolinha->x + bolinha->raio >= GetScreenWidth() ) {
                bolinha->x = GetScreenWidth() - bolinha->raio;
                bolinha->vx = -bolinha->vx * bolinha->elasticidade;
            }

            if ( bolinha->x - bolinha->raio <= 0 ) {
                bolinha->x = bolinha->raio;
                bolinha->vx = -bolinha->vx * bolinha->elasticidade;
            }

            if ( bolinha->y + bolinha->raio >= GetScreenHeight() ) {
                bolinha->y = GetScreenHeight() - bolinha->raio;
                bolinha->vy = -bolinha->vy * bolinha->elasticidade;
            }

            if ( bolinha->y - bolinha->raio <= 0 ) {
                bolinha->y = bolinha->raio;
                bolinha->vy = -bolinha->vy * bolinha->elasticidade;
            }

            bolinha->vx = bolinha->vx * bolinha->atrito;
            bolinha->vy = bolinha->vy * bolinha->atrito + gravidade;

        } else {
            bolinha->x = currX - offsetX;
            bolinha->y = currY - offsetY;
            bolinha->vx = currX - lastX;
            bolinha->vy = currY - lastY;
        }

    }

}

void processarMouse() {

    if ( mouseArrastando ) {
        lastX = currX;
        lastY = currY;
    }

    currX = GetMouseX();
    currY = GetMouseY();

    if ( IsMouseButtonPressed( 0 ) ) {

        mouseArrastando = true;

        xPress = currX;
        yPress = currY;

        for ( int i = quantidadeBolinhas-1; i >= 0; i-- ) {

            Bolinha* bolinha = bolinhas[i];

            double c1 = bolinha->x - xPress;
            double c2 = bolinha->y - yPress;
            bolinha->emArraste = c1*c1 + c2*c2 <= bolinha->raio * bolinha->raio;

            if ( bolinha->emArraste ) {
                offsetX = xPress - bolinha->x;
                offsetY = yPress - bolinha->y;
                break;
            }

        }

    }

    if ( IsMouseButtonReleased( 0 ) ) {

        mouseArrastando = false;

        for ( int i = 0; i < quantidadeBolinhas; i++ ) {
            bolinhas[i]->emArraste = false;
        }

    }

    if ( IsMouseButtonPressed( 1 ) ) {

        criarBolinha( currX, currY, 
                      GetRandomValue( 5, 25 ), 
                      GetRandomValue( 0, 1 ) == 0 ? GetRandomValue( 2, 5 ) : -GetRandomValue( 2, 5 ),
                      GetRandomValue( 0, 1 ) == 0 ? GetRandomValue( 2, 5 ) : -GetRandomValue( 2, 5 ), 
                      0.99, 0.9, 
                      GetRandomValue( 50, 255 ), GetRandomValue( 50, 255 ), GetRandomValue( 50, 255 ) );

    }

}