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

Map::Map() {
    maxWidth = 0;
    debug = false;
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

}

std::vector<Tile> &Map::getTiles() {
    return tiles;
}


void Map::loadResources() {

    int map = 2;
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

}

void Map::unloadResources() {
    for ( char c = 'A'; c <= 'Z'; c++ ) {
        UnloadTexture( tilesTexturesMap[c] );
    }
}

void Map::setDebug( bool debug ) {
    this->debug = debug;
}