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

static const double GRAVITY = .1;
static const double PLAYER_BASE_WALK_SPEED = 5;
static const double PLAYER_BASE_JUMP_SPEED = -4;
static const double TILE_WIDTH = 32;

typedef enum CollisionType {
    COLLISION_TYPE_NONE,
    COLLISION_TYPE_LEFT,
    COLLISION_TYPE_RIGHT,
    COLLISION_TYPE_TOP,
    COLLISION_TYPE_BOTTOM
} CollisionType;

typedef enum PlayerState {
    PLAYER_STATE_IDLE,
    PLAYER_STATE_WALKING,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_DOWN,
    PLAYER_STATE_DYING
} PlayerState;

typedef struct Sprite {
    Rectangle rect;
    Vector2 vel;
    Color color;
} Sprite;

typedef struct Player {
    Sprite sp;
    PlayerState state;
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
    TileMap *tileMap;
} GameWorld;

void inputAndUpdate( GameWorld *gw );
void draw( GameWorld *gw );

void updatePlayerBoundingBoxes( Player *player );
CollisionType checkCollisionPlayerVsTile( Player *player, Tile *tile );
void resolveCollisionPlayerVsTile( Player *player, Tile *tile, CollisionType collisionType );
void resolveCollisionPlayerVsTileMap( Player *player, TileMap *tileMap );

TileMap *newTileMap( char *mapData );
void freeTileMap( TileMap *tileMap );

void drawPlayer( Player *player );
void drawTile( Tile *tile );
void drawTileMap( TileMap *tileMap );

int main( void ) {

    // initialization
    const int screenWidth = 1408;
    const int screenHeight = 640;

    Player player = {
        .sp = {
            .rect = {
                .x = 100,
                .y = 400,
                .width = 28,
                .height = 40
            },
            .vel = {0},
            .color = BLUE
        },
        .state = PLAYER_STATE_IDLE,
        .bbLength = 5,
        .bbLeft = {0},
        .bbRight = {0},
        .bbTop = {0},
        .bbBottom = {0}
    };

    TileMap *tileMap = newTileMap( 
        "u\n"
        "F\n"
        "D\n"
        "D        K    K\n"
        "D      KKKKKKKK                                                                                                                                     2                      K          KKKKKKKKKKKKKKKK                   *\n"
        "D                        KKKKKKKK                                                                                                                                                                   KKKKKKKK\n"
        "D                               KKKKKK                                                                                                                                                                    KKKKKKKKKK\n"
        "D                                    KKKKKKK                                                                                                                   IIII                                       KKKKKKKKKKEBBBBBBBB\n"
        "D                                          KKKKKKK                             o o o o o o o                                                                                         8                              CAAAAAAAA\n"
        "D                                                                                                          ST                                                                    8                                  CAAAAAAAA\n"
        "D                                           o o o o o o o    3                    5      5                YQRST                                      UV                       8                                     CAAAAAAAA\n"
        "D                                   IIIIIIIIIIIIIIIIIIIIIIIIIIII        KKKKKKKKKKKKKKKKKKKKKKKKKKK       ZQRQR    1   1                             WX                    8                                        CAAAAAAAA\n"
        "D                               IIIIIIII                                          o            KKKKKKKKKKKKKKKKKKKKKKKKKKKKK                      1  WX                                                   ST o o o oCAAAAAAAA\n"
        "D                          IIIIIIII                                              o o                                                           EBBBBBBBBBBF                                               QRSTo o o CAAAAAAAA\n"
        "D        IIIIIIIIIIIIIIIIIIIIIII                     OP                         o o o                                2                      o  CAAAAAAAAAAD                                              YQRQR o o oCAAAAAAAA\n"
        "D                                                    MNOP                        o o                              o o o o                  o o CAAAAAAAAAAHBBBBBF                                        ZQRQRY o o CAAAAAAAA\n"
        "D      I                            o o o            MNMN                         o                                                      EBBBBBGAAAAAAAAAAAAAAAAHBBBBBBF                                 ZQRQRZY o oCAAAAAAAA\n"
        "D            1        1            o o o o           MNMN                    4       4       4       4                                   CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHBF         3    4    5    6      ZQRQRZZo o CAAAAAAAA\n"
        "HBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBGAAAAAAAA\n"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    );

    /*TileMap *tileMap = newTileMap( 
        "u\n"
        "F\n"
        "D\n"
        "D        K    K\n"
        "D                        KKKKKKKK                                                                                                                                     2                      K          KKKKKKKKKKKKKKKK                   *\n"
        "D                                          KKKKKKKK                                                                                                                                                                   KKKKKKKK\n"
        "D                                                 KKKKKK                                                                                                                                                                    KKKKKKKKKK\n"
        "D                                                      KKKKKKK                                                                                                                   IIII                                       KKKKKKKKKKEBBBBBBBB\n"
        "D                                                            KKKKKKK                             o o o o o o o                                                                                         8                              CAAAAAAAA\n"
        "D                                                                                                                            ST                                                                    8                                  CAAAAAAAA\n"
        "D                                                             o o o o o o o    3                    5      5                YQRST                                      UV                       8                                     CAAAAAAAA\n"
        "D                                                     IIIIIIIIIIIIIIIIIIIIIIIIIIII        KKKKKKKKKKKKKKKKKKKKKKKKKKK       ZQRQR    1   1                             WX                    8                                        CAAAAAAAA\n"
        "D                                                 IIIIIIII                                          o            KKKKKKKKKKKKKKKKKKKKKKKKKKKKK                      1  WX                                                   ST o o o oCAAAAAAAA\n"
        "D                                            IIIIIIII                                              o o                                                           EBBBBBBBBBBF                                               QRSTo o o CAAAAAAAA\n"
        "D                          IIIIIIIIIIIIIIIIIIIIIII                     OP                         o o o                                2                      o  CAAAAAAAAAAD                                              YQRQR o o oCAAAAAAAA\n"
        "D                                                                      MNOP                        o o                              o o o o                  o o CAAAAAAAAAAHBBBBBF                                        ZQRQRY o o CAAAAAAAA\n"
        "D                                                     o o o            MNMN                         o                                                      EBBBBBGAAAAAAAAAAAAAAAAHBBBBBBF                                 ZQRQRZY o oCAAAAAAAA\n"
        "D          o o o o             1        1            o o o o           MNMN                    4       4       4       4                                   CAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHBF         3    4    5    6      ZQRQRZZo o CAAAAAAAA\n"
        "HBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBGAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBGAAAAAAAA\n"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    );*/

    GameWorld gw = {
        .player = &player,
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

    if ( IsKeyPressed( KEY_Q ) ) {
        player->sp.rect.x = 100;
        player->sp.rect.x = 400;
        player->sp.vel = (Vector2){0};
    }
    
    if ( IsKeyPressed( KEY_SPACE ) && 
         ( player->state == PLAYER_STATE_IDLE || 
           player->state == PLAYER_STATE_WALKING ) ) {
        player->sp.vel.y = PLAYER_BASE_JUMP_SPEED;
        player->state = PLAYER_STATE_JUMPING;
    }

    if ( IsKeyDown( KEY_LEFT ) ) {
        player->sp.vel.x = -PLAYER_BASE_WALK_SPEED;
        player->state = PLAYER_STATE_WALKING;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        player->sp.vel.x = PLAYER_BASE_WALK_SPEED;
        player->state = PLAYER_STATE_WALKING;
    } else {
        player->sp.vel.x = 0;
        player->state = PLAYER_STATE_IDLE;
    }

    /*if ( IsKeyDown( KEY_UP ) ) {
        player->sp.vel.y = -PLAYER_BASE_WALK_SPEED;
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        player->sp.vel.y = PLAYER_BASE_WALK_SPEED;
    } else {
        player->sp.vel.y = 0;
    }*/

    player->sp.vel.y += GRAVITY;
    
    player->sp.rect.x += player->sp.vel.x;
    player->sp.rect.y += player->sp.vel.y;

    updatePlayerBoundingBoxes( player );
    resolveCollisionPlayerVsTileMap( player, gw->tileMap );

}

void draw( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawTileMap( gw->tileMap );
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

void resolveCollisionPlayerVsTile( Player *player, Tile *tile, CollisionType collisionType ) {

    switch ( collisionType ) {
        case COLLISION_TYPE_LEFT:
            player->sp.rect.x = tile->sp.rect.x + tile->sp.rect.width;
            break;
        case COLLISION_TYPE_RIGHT:
            player->sp.rect.x = tile->sp.rect.x - player->sp.rect.width;
            break;
        case COLLISION_TYPE_TOP:
            player->sp.rect.y = tile->sp.rect.y + tile->sp.rect.height;
            player->sp.vel.y = 0;
            break;
        case COLLISION_TYPE_BOTTOM:
            player->sp.rect.y = tile->sp.rect.y - player->sp.rect.height;
            player->sp.vel.y = 0;
            player->state = PLAYER_STATE_IDLE;
            break;
    }

    if ( collisionType != COLLISION_TYPE_NONE ) {
        updatePlayerBoundingBoxes( player );
    }

}

void resolveCollisionPlayerVsTileMap( Player *player, TileMap *tileMap ) {

    for ( int i = 0; i < tileMap->rows; i++ ) {
        for ( int j = 0; j < tileMap->columns; j++ ) {
            Tile *tile = &tileMap->tiles[i*tileMap->columns+j];
            if ( tile != NULL ) {
                CollisionType collisionType = checkCollisionPlayerVsTile( 
                    player, tile );
                if ( collisionType != COLLISION_TYPE_NONE ) {
                    resolveCollisionPlayerVsTile( player, tile, collisionType );                    
                }
            }
        }
    }

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
                            .x = j * TILE_WIDTH,
                            .y = i * TILE_WIDTH,
                            .width = TILE_WIDTH,
                            .height = TILE_WIDTH
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