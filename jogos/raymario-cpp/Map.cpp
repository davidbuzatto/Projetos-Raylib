/**
 * @file Map.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Map class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Map.h>

#include <vector>
#include <string>
#include <iostream>
#include <raylib.h>
#include <Coin.h>
#include <Goomba.h>
#include <Sprite.h>

Map::Map() :
    maxWidth( 0 ) {
}

Map::~Map() {
    unloadResources();
}

void Map::draw() {

    int repeats = maxWidth / backgroundTexture.width;
    for ( int i = 0; i <= repeats; i++ ) {
        DrawTexture( backgroundTexture, i*backgroundTexture.width, 0, WHITE );
    }

    for ( size_t i = 0; i < tiles.size(); i++ ) {
        tiles[i].draw();
    }

    for ( size_t i = 0; i < coins.size(); i++ ) {
        coins[i].draw();
    }

    for ( size_t i = 0; i < goombas.size(); i++ ) {
        goombas[i].draw();
    }

}

std::vector<Tile> &Map::getTiles() {
    return tiles;
}

std::vector<Coin> &Map::getCoins() {
    return coins;
}

std::vector<Goomba> &Map::getGoombas() {
    return goombas;
}

void Map::loadResources() {

    int map = 1;
    for ( char c = 'A'; c <= 'Z'; c++ ) {
        tilesTexturesMap[c] = LoadTexture( TextFormat( "resources/images/tiles/tile_%c.png", c ) );
    }

    backgroundTexture = LoadTexture( TextFormat( "resources/images/backgrounds/background%d.png", map ) );

    char *mapData = LoadFileText( TextFormat( "resources/maps/map%d.txt", map ) );
    float tileWidth = 32;
    int currentColumn = 0;
    int currentLine = 0;

    while ( *mapData != '\0' ) {

        float x = currentColumn*tileWidth;
        float y = currentLine*tileWidth;

        if ( maxWidth < x ) {
            maxWidth = x;
        }

        switch ( *mapData ) {
            case 'a':
                tiles.push_back( 
                    Tile( 
                        Vector2( x, y ), 
                        Vector2( tileWidth, tileWidth ), 
                        GREEN,
                        nullptr
                    )
                );
                break;
            case 'b':
                tiles.push_back( 
                    Tile( 
                        Vector2( x, y ), 
                        Vector2( tileWidth, tileWidth ), 
                        BLUE,
                        nullptr
                    )
                );
                break;
            case 'c':
                tiles.push_back( 
                    Tile( 
                        Vector2( x, y ), 
                        Vector2( tileWidth, tileWidth ), 
                        RED,
                        nullptr
                    )
                );
                break;
            case 'd':
                tiles.push_back( 
                    Tile( 
                        Vector2( x, y ), 
                        Vector2( tileWidth, tileWidth ), 
                        ORANGE,
                        nullptr
                    )
                );
                break;
            case 'o':
                coins.push_back( Coin( Vector2( x, y ), Vector2( 25, 32 ), YELLOW ) );
                break;
            case '1':
                goombas.push_back( Goomba( Vector2( x, y+2 ), Vector2( -100, 0 ), Vector2( 32, 30 ), YELLOW ) );
                break;
            case '\n':
                currentLine++;
                currentColumn = -1;
                break;
            case ' ':
                break;
            default:
                int index = (*mapData)-'A';
                if ( index >= 0 && index <= 26 ) {
                    tiles.push_back( 
                        Tile( 
                            Vector2( x, y ), 
                            Vector2( tileWidth, tileWidth ), 
                            BLACK,
                            &tilesTexturesMap[*mapData]
                        )
                    );
                }
                break;
        }

        currentColumn++;
        mapData++;
        
    }

    Coin::loadResources();
    Goomba::loadResources();

}

void Map::unloadResources() {
    for ( char c = 'A'; c <= 'Z'; c++ ) {
        UnloadTexture( tilesTexturesMap[c] );
    }
    Coin::unloadResources();
    Goomba::unloadResources();
}