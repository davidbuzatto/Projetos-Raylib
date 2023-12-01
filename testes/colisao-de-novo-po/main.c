/*******************************************************************************
 * Mais uma tentativa de implementação de detecção de colisão.
 * 
 * Author: Prof. Dr. David Buzatto
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"
#include "include/raymath.h"

typedef enum CollisionType {
    COLLISION_TYPE_NONE,
    COLLISION_TYPE_LEFT,
    COLLISION_TYPE_RIGHT,
    COLLISION_TYPE_TOP,
    COLLISION_TYPE_BOTTOM
} CollisionType;

typedef struct Sprite {
    Rectangle rect;
    Vector2 vel;
    Color color;
} Sprite;

typedef struct Player {
    Sprite sp;
    int bbLength;
    Rectangle bbLeft;
    Rectangle bbRight;
    Rectangle bbTop;
    Rectangle bbBottom;
} Player;

typedef struct Tile {
    Sprite sp;
} Tile;

typedef struct TileMap {
    int rows;
    int columns;
    Tile *tiles;
} TileMap;

typedef struct {
    Player *player;
    Tile *tile;
    TileMap *tileMap;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void updatePlayerBoundingBoxes( Player *player );
CollisionType checkCollisionPlayerVsTile( Player *player, Tile *tile );
void resolveCollisionPlayerVsTile( Player *player, Tile *tile );

TileMap *newTileMap( char *mapData );
void freeTileMap( TileMap *tileMap );

void drawPlayer( Player *player );
void drawTile( Tile *tile );
void drawTileMap( TileMap *tileMap );

int main( void ) {

    // initialization
    const int screenWidth = 800;
    const int screenHeight = 480;

    Player player = {
        .sp = {
            .rect = {
                .x = 100,
                .y = 100,
                .width = 28,
                .height = 40
            },
            .vel = {0},
            .color = BLUE
        },
        .bbLength = 5,
        .bbLeft = {0},
        .bbRight = {0},
        .bbTop = {0},
        .bbBottom = {0}
    };

    Tile tile = {
        .sp = {
            .rect = {
                .x = 200,
                .y = 100,
                .width = 32,
                .height = 32
            },
            .vel = {0},
            .color = BLACK
        }
    };

    TileMap *tileMap = newTileMap( 
        "#            $$$$$$$    #\n"
        "#                       #\n"
        "#                       #\n"
        "#       $$$$$$$         #\n"
        "#                       #\n"
        "#                       #\n"
        "#                 %%%%% #\n"
        "#                       #\n"
        "#                       #\n"
        "#              %%%      #\n"
        "#                       #\n"
        "#                       #\n"
        "#           ###         #\n"
        "#         #######       #\n"
        "#########################"
    );

    GameWorld gw = {
        .player = &player,
        .tile = &tile,
        .tileMap = tileMap
    };

    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Mais um teste de colisão..." );
    SetTargetFPS( 60 );    

    while ( !WindowShouldClose() ) {
        inputAndUpdate( &gw );
        draw( &gw );
    }

    CloseWindow();
    freeTileMap( tileMap );

    return 0;

}

void inputAndUpdate( GameWorld *gw ) {

    Player *player = gw->player;
    Tile *tile = gw->tile;

    if ( IsKeyDown( KEY_LEFT ) ) {
        player->sp.vel.x = -5;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        player->sp.vel.x = 5;
    } else {
        player->sp.vel.x = 0;
    }

    if ( IsKeyDown( KEY_UP ) ) {
        player->sp.vel.y = -5;
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        player->sp.vel.y = 5;
    } else {
        player->sp.vel.y = 0;
    }

    player->sp.rect.x += player->sp.vel.x;
    player->sp.rect.y += player->sp.vel.y;

    updatePlayerBoundingBoxes( player );
    resolveCollisionPlayerVsTile( player, tile );

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawTileMap( gw->tileMap );
    drawTile( gw->tile );
    drawPlayer( gw->player );

    EndDrawing();

}

void updatePlayerBoundingBoxes( Player *player ) {

    Rectangle *r = &player->sp.rect;

    player->bbLeft = (Rectangle){
        .x = r->x,
        .y = r->y + player->bbLength,
        .width = player->bbLength,
        .height = r->height - 2 * player->bbLength
    };

    player->bbRight = (Rectangle){
        .x = r->x + r->width - player->bbLength,
        .y = r->y + player->bbLength,
        .width = player->bbLength,
        .height = r->height - 2 * player->bbLength
    };

    player->bbTop = (Rectangle){
        .x = r->x + player->bbLength,
        .y = r->y,
        .width = r->width - 2 * player->bbLength,
        .height = player->bbLength
    };

    player->bbBottom = (Rectangle){
        .x = r->x + player->bbLength,
        .y = r->y + r->height - player->bbLength,
        .width = r->width - 2 * player->bbLength,
        .height = player->bbLength
    };

}

CollisionType checkCollisionPlayerVsTile( Player *player, Tile *tile ) {

    if ( CheckCollisionRecs( player->bbLeft, tile->sp.rect ) ) {
        return COLLISION_TYPE_LEFT;
    } else if ( CheckCollisionRecs( player->bbRight, tile->sp.rect ) ) {
        return COLLISION_TYPE_RIGHT;
    }

    if ( CheckCollisionRecs( player->bbTop, tile->sp.rect ) ) {
        return COLLISION_TYPE_TOP;
    } else if ( CheckCollisionRecs( player->bbBottom, tile->sp.rect ) ) {
        return COLLISION_TYPE_BOTTOM;
    }

    return COLLISION_TYPE_NONE;

}

void resolveCollisionPlayerVsTile( Player *player, Tile *tile ) {

    switch ( checkCollisionPlayerVsTile( player, tile ) ) {
        case COLLISION_TYPE_LEFT:
            player->sp.rect.x = tile->sp.rect.x + tile->sp.rect.width;
            break;
        case COLLISION_TYPE_RIGHT:
            player->sp.rect.x = tile->sp.rect.x - player->sp.rect.width;
            break;
        case COLLISION_TYPE_TOP:
            player->sp.rect.y = tile->sp.rect.y + tile->sp.rect.height;
            break;
        case COLLISION_TYPE_BOTTOM:
            player->sp.rect.y = tile->sp.rect.y - player->sp.rect.height;
            break;
    }

    updatePlayerBoundingBoxes( player );

}

TileMap *newTileMap( char *mapData ) {

    int rowCount = 1;
    int colCount = 0;
    int maxColCount = -1;
    char *c = mapData;

    TileMap *tileMap = NULL;
    Tile *tiles = NULL;

    while ( *c != '\0' ) {
        if ( *c != '\n' ) {
            colCount++;
        } else {
            if ( maxColCount < colCount ) {
                maxColCount = colCount;
            }
            colCount = 0;
            rowCount++;
        }
        c++;
    }

    if ( maxColCount < colCount ) {
        maxColCount = colCount;
    }

    tileMap = (TileMap*) malloc( sizeof( TileMap ) );
    tileMap->rows = rowCount;
    tileMap->columns = maxColCount;

    tiles = (Tile*) malloc( ( tileMap->rows * tileMap->columns ) * sizeof( Tile ) );
    memset( tiles, 0, tileMap->rows * tileMap->columns );
    tileMap->tiles = tiles;

    c = mapData;
    int i = 0;
    int j = 0;
    while ( *c != '\0' ) {
        if ( *c != '\n' ) {
            if ( *c != ' ' ) {

                Color color = BLACK;

                switch ( *c ) {
                    case '#':
                        color = BLACK;
                        break;
                    case '$':
                        color = ORANGE;
                        break;
                    case '%':
                        color = LIME;
                        break;
                }

                tiles[i*tileMap->columns+j] = (Tile){
                    .sp = {
                        .rect = {
                            .x = j * 32,
                            .y = i * 32,
                            .width = 32,
                            .height = 32
                        },
                        .vel = {0},
                        .color = color
                    }
                };

            }
            j++;
        } else {
            j = 0;
            i++;
        }
        c++;
    }

    return tileMap;

}

void freeTileMap( TileMap *tileMap ) {
    free( tileMap->tiles );
    free( tileMap );
}

void drawPlayer( Player *player ) {

    DrawRectangle( 
        player->sp.rect.x, 
        player->sp.rect.y, 
        player->sp.rect.width, 
        player->sp.rect.height, 
        player->sp.color );

    DrawRectangleRec( player->bbLeft, GREEN );
    DrawRectangleRec( player->bbRight, ORANGE );
    DrawRectangleRec( player->bbTop, RED );
    DrawRectangleRec( player->bbBottom, VIOLET );

}

void drawTile( Tile *tile ) {
    if ( tile != NULL ) {
        DrawRectangle( 
            tile->sp.rect.x, 
            tile->sp.rect.y, 
            tile->sp.rect.width, 
            tile->sp.rect.height, 
            tile->sp.color );
    }
}

void drawTileMap( TileMap *tileMap ) {

    for ( int i = 0; i < tileMap->rows; i++ ) {
        for ( int j = 0; j < tileMap->columns; j++ ) {
            drawTile( &(tileMap->tiles[i*tileMap->columns+j]) );
        }
    }

}