/*******************************************************************************
 * Puzzle Bobble using Raylib (https://www.raylib.com/)
 * 
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

const int BALL_RADIUS = 25;
const int BALL_SPEED = 10;
const double ANGLE_INC = 0.5;

typedef struct Ball {
    int id;
    Vector2 pos;
    Vector2 vel;
    int speed;
    int radius;
    double angle;
    int xBounceCount;
    double xLimits[10];
    double yLimits[10];
    Color color;
    Texture2D texture;
    bool visible;
    bool selectedForRemoval;
} Ball;

int currentBallId;
Ball *movingBall;
Color BALL_COLORS[8];
Texture2D BALL_TEXTURES[8];
Texture2D backgroundTexture;
Ball *balls[200];
int ballQuantity;
int colorQuantity = 4;

bool graph[200][200];
bool visited[200];
int componentIds[200];
bool removing;

// debug
int xt = -1;
int yt = -1;

typedef struct Cannon {
    Vector2 pos;
    int aimSightLength;
    double angle;
    Color color;
    Ball *currentBall;
    Ball *nextBall;
    double minAngle;
    double maxAngle;
} Cannon;

typedef struct {
    Cannon *cannon;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void createInitialBalls( void );
Ball newBall( int x, int y, Color color, Texture2D texture, bool visible );
void newCannonBall( Cannon *cannon );
void drawBall( Ball *ball );
void drawBalls( void );
void drawCannon( Cannon *cannon );
bool ballIntercepts( Ball *b1, Ball *b2, int offset );
bool interceptBalls( Ball *b, int *interceptedIndex );

double toRadians( double degrees );
double toDegrees( double radians );

void resetGraphDS( void );
void drawGraph( void );
void dfsCollectBalls( int sourceId, int baseId );
void identifyConnectedComponents();
void dfsConnectedComponents( int sourceId, int componentId );
bool colorEquals( Color c1, Color c2 );

bool colorEquals( Color c1, Color c2 ) {
    return c1.r == c2.r &&
         c1.g == c2.g &&
         c1.b == c2.b;
}

void dfsCollectBalls( int sourceId, int baseId ) {

    Ball *ball = balls[baseId];
    ball->selectedForRemoval = true;
    Color baseColor = ball->color;
    
    if ( !visited[sourceId] && balls[sourceId]->visible ) {
        visited[sourceId] = true;
        // adj
        for ( int i = 0; i < ballQuantity; i++ ) {
            if ( graph[sourceId][i] == true && 
                 colorEquals( baseColor, balls[i]->color ) ) {
                balls[i]->selectedForRemoval = true;
                dfsCollectBalls( i, baseId );
            }
        }
    }

}

void identifyConnectedComponents() {

    int componentId = 0;

    for ( int i = 0; i < ballQuantity; i++ ) {
        if ( !visited[i] ) {
            dfsConnectedComponents( i, componentId );
            componentId++;
        }
    }

}

void dfsConnectedComponents( int sourceId, int componentId ) {
    if ( !visited[sourceId] ) {
        visited[sourceId] = true;
        for ( int i = 0; i < ballQuantity; i++ ) {
            if ( graph[sourceId][i] == true ) {
                componentIds[i] = componentId;
                dfsConnectedComponents( i, componentId );
            }
        }
    }
}

int main( void ) {

    BALL_COLORS[0] = BLUE;
    BALL_COLORS[1] = GREEN;
    BALL_COLORS[2] = YELLOW;
    BALL_COLORS[3] = RED;
    BALL_COLORS[4] = PURPLE;
    BALL_COLORS[5] = ORANGE;
    BALL_COLORS[6] = GRAY;
    BALL_COLORS[7] = BLACK;

    resetGraphDS();

    const int screenWidth = 400;
    const int screenHeight = 700;

    Cannon cannon = {
        .pos = {
            .x = screenWidth / 2,
            .y = screenHeight - 100
        },
        .aimSightLength = screenHeight,
        .angle = 270,
        .minAngle = 190,
        .maxAngle = 350,
        .color = WHITE,
        .currentBall = NULL,
        .nextBall = NULL
    };

    GameWorld gw = {
        .cannon = &cannon
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Puzzle Bobble" );
    //InitAudioDevice();
    SetTargetFPS( 60 );    

    BALL_TEXTURES[0] = LoadTexture( "resources/blueBall.png" );
    BALL_TEXTURES[1] = LoadTexture( "resources/greenBall.png" );
    BALL_TEXTURES[2] = LoadTexture( "resources/yellowBall.png" );
    BALL_TEXTURES[3] = LoadTexture( "resources/redBall.png" );
    BALL_TEXTURES[4] = LoadTexture( "resources/purpleBall.png" );
    BALL_TEXTURES[5] = LoadTexture( "resources/orangeBall.png" );
    BALL_TEXTURES[6] = LoadTexture( "resources/grayBall.png" );
    BALL_TEXTURES[7] = LoadTexture( "resources/blackBall.png" );
    backgroundTexture = LoadTexture( "resources/background.png" );

    currentBallId = 0;
    removing = false;

    createInitialBalls();
    newCannonBall( &cannon );
    newCannonBall( &cannon );

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    // unload resources...
    // free objects

    //CloseAudioDevice();
    CloseWindow();

    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    Cannon *cannon = gw->cannon;

    if ( IsKeyDown( KEY_RIGHT ) ) {
        if ( cannon->angle < cannon->maxAngle ) {
            cannon->angle += ANGLE_INC;
        } else {
            cannon->angle = cannon->maxAngle;
        }
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        if ( cannon->angle > cannon->minAngle ) {
            cannon->angle -= ANGLE_INC;
        } else {
            cannon->angle = cannon->minAngle;
        }
    }

    if ( IsKeyPressed( KEY_SPACE ) && movingBall == NULL && !removing ) {
        movingBall = cannon->currentBall;
        movingBall->pos.x = cannon->pos.x;
        movingBall->pos.y = cannon->pos.y;
        movingBall->vel.x = movingBall->speed * cos( toRadians( cannon->angle ) );
        movingBall->vel.y = movingBall->speed * sin( toRadians( cannon->angle ) );
        movingBall->angle = cannon->angle;
        movingBall->xBounceCount = 0;
        newCannonBall( cannon );
    }

    if ( movingBall != NULL ) {

        movingBall->pos.x += movingBall->vel.x;
        movingBall->pos.y += movingBall->vel.y;
        int interceptedIndex = -1;

        if ( movingBall->pos.x + movingBall->radius >= GetScreenWidth() ) {
            movingBall->xBounceCount++;
            //ball->pos.x = GetScreenWidth() - ball->radius;
            movingBall->pos.x = movingBall->xLimits[movingBall->xBounceCount-1];
            movingBall->pos.y = movingBall->yLimits[movingBall->xBounceCount-1];
            movingBall->vel.x = -movingBall->vel.x;
        } else if ( movingBall->pos.x - movingBall->radius <= 0 ) {
            movingBall->xBounceCount++;
            //ball->pos.x = ball->radius;
            movingBall->pos.x = movingBall->xLimits[movingBall->xBounceCount-1];
            movingBall->pos.y = movingBall->yLimits[movingBall->xBounceCount-1];
            movingBall->vel.x = -movingBall->vel.x;
        } else if ( movingBall->pos.y - movingBall->radius <= 0 ) {
            // top container collision
            movingBall->vel.x = 0;
            movingBall->vel.y = 0;
            movingBall->pos.y = movingBall->radius;
            balls[ballQuantity++] = movingBall;
            movingBall = NULL;
        } else if ( interceptBalls( movingBall, &interceptedIndex ) ) {

            movingBall->vel.x = 0;
            movingBall->vel.y = 0;
            balls[ballQuantity++] = movingBall;
            Ball *interceptedBall = balls[interceptedIndex];
            
            double relAng = atan2( 
                movingBall->pos.y - interceptedBall->pos.y,
                movingBall->pos.x - interceptedBall->pos.x );
            relAng = toDegrees( relAng );

            double pAngle = 0;

            if ( relAng >= -30 && relAng < 30 ) {
                pAngle = toRadians( 0 );
            } else if ( relAng >= 30 && relAng < 90 ) {
                pAngle = toRadians( 60 );
            } else if ( relAng >= 90 && relAng < 150 ) {
                pAngle = toRadians( 120 );
            } else if ( relAng >= 150 && relAng < 210 ) {
                pAngle = toRadians( -180 );
            } else if ( relAng >= 210 && relAng < 270 ) {
                pAngle = toRadians( -120 );
            } else {
                pAngle = toRadians( -60 );
            }

            movingBall->pos.x = interceptedBall->pos.x + BALL_RADIUS * 2 * cos( pAngle );
            movingBall->pos.y = interceptedBall->pos.y + BALL_RADIUS * 2 * sin( pAngle );

            for ( int i = 0; i < ballQuantity-1; i++ ) {
                if ( balls[i]->visible && ballIntercepts( movingBall, balls[i], 5 ) ) {
                    graph[i][movingBall->id] = true;
                    graph[movingBall->id][i] = true;
                }
            }

            memset( visited, 0, 200 );
            dfsCollectBalls( movingBall->id, movingBall->id );

            int removeQuantity = 0;
            for ( int i = 0; i < ballQuantity; i++ ) {
                if ( balls[i]->selectedForRemoval ) {
                    removeQuantity++;
                }
            }
            movingBall = NULL;

            if ( removeQuantity >= 3 ) {
                
                for ( int i = 0; i < ballQuantity; i++ ) {
                    Ball *ball = balls[i];
                    if ( ball->selectedForRemoval ) {
                        for ( int j = 0; j < ballQuantity; j++ ) {
                            Ball *ballj = balls[j];
                            graph[ball->id][ballj->id] = false;
                            graph[ballj->id][ball->id] = false;
                        }
                    }
                    ball->vel.y = GetRandomValue( 0.5, 2 );
                }

                memset( visited, 0, 200 );
                memset( componentIds, -1, 200 );
                identifyConnectedComponents();
                for ( int i = 0; i < ballQuantity; i++ ) {
                    printf( "(%d) %d\n", i, componentIds[i] );
                }
                printf( "\n" );

                for ( int i = 0; i < ballQuantity; i++ ) {
                    if ( componentIds[i] != 0 ) {
                        Ball *ball = balls[i];
                        ball->selectedForRemoval = true;
                        ball->vel.y = GetRandomValue( 0.5, 2 );
                    }
                }

            } else {
                for ( int i = 0; i < ballQuantity; i++ ) {
                    balls[i]->selectedForRemoval = false;
                }
            }

        }

    }

    int q = 0;
    for ( int i = 0; i < ballQuantity; i++ ) {
        Ball *ball = balls[i];
        if ( ball->selectedForRemoval ) {
            removing = true;
            ball->pos.y += ball->vel.y;
            ball->vel.y += 0.5;
            q++;
        }
        if ( ball->pos.y - BALL_RADIUS > GetScreenHeight() + 50 ) {
            ball->selectedForRemoval = false;
        }
    }

    if ( q == 0 ) {
        removing = false;
    }

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    DrawTexture( backgroundTexture, 0, 0, WHITE );
    drawCannon( gw->cannon );
    drawBalls();

    if ( xt >= 0 && yt >= 0 ) {
        DrawCircle( xt, yt, 10, BLACK );
    }

    //drawGraph();

    EndDrawing();

}

void createInitialBalls( void ) {

    Color colors[4];
    colors[0] = RED; 
    colors[1] = YELLOW; 
    colors[2] = BLUE; 
    colors[3] = GREEN;

    Texture2D textures[4];
    textures[0] = BALL_TEXTURES[3]; 
    textures[1] = BALL_TEXTURES[2]; 
    textures[2] = BALL_TEXTURES[0]; 
    textures[3] = BALL_TEXTURES[1];

    for ( int i = 0; i < 7; i++ ) {
        Ball *ball = (Ball*) malloc( sizeof( Ball ) );
        *ball = newBall( BALL_RADIUS * 2 + i * BALL_RADIUS * 2, -18, WHITE, textures[i/2], false );
        balls[ballQuantity++] = ball;
    }

    double xStart = balls[ballQuantity-7]->pos.x + BALL_RADIUS * 2 * cos( toRadians( 120 ) );
    double yStart = balls[ballQuantity-7]->pos.y + BALL_RADIUS * 2 * sin( toRadians( 120 ) );

    for ( int i = 0; i < 8; i++ ) {
        Ball *ball = (Ball*) malloc( sizeof( Ball ) );
        *ball = newBall( xStart + i * BALL_RADIUS * 2, yStart, colors[i/2], textures[i/2], true );
        balls[ballQuantity++] = ball;
    }

    xStart = balls[ballQuantity-8]->pos.x + BALL_RADIUS * 2 * cos( toRadians( 60 ) );
    yStart = balls[ballQuantity-8]->pos.y + BALL_RADIUS * 2 * sin( toRadians( 60 ) );

    for ( int i = 0; i < 7; i++ ) {
        Ball *ball = (Ball*) malloc( sizeof( Ball ) );
        *ball = newBall( xStart + i * BALL_RADIUS * 2, yStart, colors[i/2], textures[i/2], true );
        balls[ballQuantity++] = ball;
    }

    colors[0] = BLUE; 
    colors[1] = GREEN; 
    colors[2] = RED; 
    colors[3] = YELLOW;

    textures[0] = BALL_TEXTURES[0]; 
    textures[1] = BALL_TEXTURES[1]; 
    textures[2] = BALL_TEXTURES[3]; 
    textures[3] = BALL_TEXTURES[2];

    xStart = balls[ballQuantity-7]->pos.x + BALL_RADIUS * 2 * cos( toRadians( 120 ) );
    yStart = balls[ballQuantity-7]->pos.y + BALL_RADIUS * 2 * sin( toRadians( 120 ) );

    for ( int i = 0; i < 8; i++ ) {
        Ball *ball = (Ball*) malloc( sizeof( Ball ) );
        *ball = newBall( xStart + i * BALL_RADIUS * 2, yStart, colors[i/2], textures[i/2], true );
        balls[ballQuantity++] = ball;
    }

    xStart = balls[ballQuantity-8]->pos.x + BALL_RADIUS * 2 * cos( toRadians( 60 ) );
    yStart = balls[ballQuantity-8]->pos.y + BALL_RADIUS * 2 * sin( toRadians( 60 ) );

    for ( int i = 0; i < 7; i++ ) {
        Ball *ball = (Ball*) malloc( sizeof( Ball ) );
        *ball = newBall( xStart + i * BALL_RADIUS * 2, yStart, colors[i/2], textures[i/2], true );
        balls[ballQuantity++] = ball;
    }

    // graph induction
    for ( int i = 0; i < ballQuantity; i++ ) {
        for ( int j = i+1; j < ballQuantity; j++ ) {
            if ( balls[j]->visible && ballIntercepts( balls[i], balls[j], 5 ) ) {
                graph[balls[i]->id][balls[j]->id] = true;
                graph[balls[j]->id][balls[i]->id] = true;
            }
        }
    }


}

Ball newBall( int x, int y, Color color, Texture2D texture, bool visible ) {

    return (Ball){
        .id = currentBallId++,
        .pos = {
            .x = x,
            .y = y
        },
        .vel = {
            .x = 0,
            .y = 0
        },
        .speed = BALL_SPEED,
        .radius = BALL_RADIUS,
        .angle = 0,
        .xBounceCount = 0,
        .color = color,
        .texture = texture,
        .visible = visible,
        .selectedForRemoval = false
    };

}
void newCannonBall( Cannon *cannon ) {

    Ball *ball = (Ball*) malloc( sizeof( Ball ) );
    int index = GetRandomValue( 0, colorQuantity-1 );

    *ball = newBall( 
        cannon->pos.x - 100, 
        cannon->pos.y + 50, 
        BALL_COLORS[index], 
        BALL_TEXTURES[index], 
        true );

    cannon->currentBall = cannon->nextBall;
    cannon->nextBall = ball;

    if ( cannon->currentBall != NULL ) {
        cannon->currentBall->pos.x = cannon->pos.x;
        cannon->currentBall->pos.y = cannon->pos.y;
    }

}

void drawBall( Ball *ball ) {

    if ( ball->visible ) {
        //DrawCircle( ball->pos.x, ball->pos.y, ball->radius, ball->color );
        DrawTexture( ball->texture, ball->pos.x - ball->radius, ball->pos.y - ball->radius, WHITE );
    }

    /*if ( ball->selectedForRemoval ) {
        DrawCircle( ball->pos.x, ball->pos.y, 10, WHITE );
    }

    char idStr[5];
    sprintf( idStr, "%d", ball->id );
    DrawText( idStr, ball->pos.x, ball->pos.y, 15, BLACK );*/

    /*char str[20];
    sprintf( str, "bc: %d", ball->xBounceCount );
    DrawText( str, ball->pos.x, ball->pos.y, 10, BLACK );*/

}

void drawBalls( void ) {
    for ( int i = 0; i < ballQuantity; i++ ) {
        drawBall( balls[i] );
    }
}

void drawCannon( Cannon *cannon ) {

    double x = cannon->pos.x;
    double y = cannon->pos.y;
    double xEnd;
    double yEnd;
    double ra;
    double reflectLength;
    double ca;
    double co;
    Ball *ball = cannon->currentBall;

    for ( int i = 0; i < 10; i++ ) {

        if ( i % 2 == 0 ) {
            ra = toRadians( cannon->angle );
        } else {
            ra = toRadians( -180 - cannon->angle );
        }

        if ( i == 0 ) {
            reflectLength = cannon->pos.x - ball->radius;
        } else {
            reflectLength = GetScreenWidth() - ball->radius * 2;
        }
        
        if ( cannon->angle >= 270 ) {
            ca = reflectLength;
            co = reflectLength * tan(ra);
        } else {
            ca = -reflectLength;
            co = -reflectLength * tan(ra);
        }

        if ( i % 2 == 0 ) {
            xEnd = x + ca;
            yEnd = y + co;
        } else {
            xEnd = x - ca;
            yEnd = y - co;
        }

        ball->xLimits[i] = xEnd;
        ball->yLimits[i] = yEnd;

        DrawLine( 
            (int) x, 
            (int) y, 
            (int) xEnd, 
            (int) yEnd,
            cannon->color );

        //DrawCircle( (int) xEnd, (int) yEnd, 10, GREEN );

        x = xEnd;
        y = yEnd;

    }

    drawBall( cannon->currentBall );
    drawBall( cannon->nextBall );

    if ( movingBall != NULL ) {
        drawBall( movingBall );
    }

}

void drawGraph( void ) {

    for ( int i = 0; i < ballQuantity; i++ ) {
        for ( int j = 0; j < ballQuantity; j++ ) {
            if ( graph[i][j] ) {
                Ball *b1 = balls[i];
                Ball *b2 = balls[j];
                DrawLine( b1->pos.x, b1->pos.y, b2->pos.x, b2->pos.y, BLACK );
            }
        }
    }

}

bool ballIntercepts( Ball *b1, Ball *b2, int offset ) {

    if ( b1 == NULL ) {
        return false;
    }

    if ( b2 == NULL ) {
        return false;
    }

    double c1 = b1->pos.x - b2->pos.x;
    double c2 = b1->pos.y - b2->pos.y;
    double d = hypot( c1, c2 );

    // should simplify? (not using square root)
    return d - offset <= b1->radius * 2;

}

bool interceptBalls( Ball *b, int *interceptedIndex ) {
    for ( int i = 0; i < ballQuantity; i++ ) {
        if ( ballIntercepts( b, balls[i], 0 ) ) {
            *interceptedIndex = i;
            return true;
        }
    }
    return false;
}

double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}

void resetGraphDS( void ) {
    memset( graph, 0, 200 * 200 );
    memset( visited, 0, 200 );
    memset( componentIds, -1, 200 );
}