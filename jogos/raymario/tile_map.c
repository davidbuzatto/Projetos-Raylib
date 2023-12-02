#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"
#include "include/raymath.h"

#include "main.h"

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
                Texture2D *texture = NULL;
                bool collideable = false;
                bool visible = true;

                if ( *c >= 'A' && *c <= 'Z' ) {
                    texture = &TILE_TEXTURE_CACHE[(int) (*c-'A')];
                    collideable = true;
                } else {
                    switch ( *c ) {
                        case '#':
                            color = BLACK;
                            collideable = true;
                            visible = false;
                            break;
                        case '$':
                            color = ORANGE;
                            break;
                        case '%':
                            color = LIME;
                            break;
                    }
                    visible = false;
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
                    },
                    .texture = texture,
                    .collideable = collideable,
                    .visible = visible
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

void drawTile( Tile *tile ) {
    if ( tile != NULL && tile->visible ) {
        if ( tile->texture != NULL ) {
            DrawTexture( 
                *(tile->texture), 
                tile->sp.rect.x, 
                tile->sp.rect.y, 
                WHITE );
        } else {
            DrawRectangle( 
                tile->sp.rect.x, 
                tile->sp.rect.y, 
                tile->sp.rect.width, 
                tile->sp.rect.height, 
                tile->sp.color );
        }
    }
}

void drawTileMap( TileMap *tileMap ) {

    for ( int i = 0; i < tileMap->columns * TILE_WIDTH / CURRENT_BACKGROUND.width; i++ ) {
        DrawTexture( CURRENT_BACKGROUND, i * CURRENT_BACKGROUND.width, 0, WHITE );
    }

    for ( int i = 0; i < tileMap->rows; i++ ) {
        for ( int j = 0; j < tileMap->columns; j++ ) {
            drawTile( &(tileMap->tiles[i*tileMap->columns+j]) );
        }
    }

}

void loadTileTextureCache( void ) {
    
    char texturePath[100];

    for ( int i = 0; i < 26; i++ ) {
        sprintf( texturePath, "resources/images/tiles/tile_%c.png", (char) ('A' + i ) );
        TILE_TEXTURE_CACHE[i] = LoadTexture( texturePath );
    }

}

void unloadTileTextureCache( void ) {
    
    for ( int i = 0; i < 26; i++ ) {
        UnloadTexture( TILE_TEXTURE_CACHE[i] );
    }

}

void loadBackground( void ) {
    CURRENT_BACKGROUND = LoadTexture( "resources/images/backgrounds/background0.png" );
}

void unloadBackground( void ) {
    UnloadTexture( CURRENT_BACKGROUND );
}