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
}

Map::~Map() {
    unloadResources();
}

void Map::draw() {

    for ( size_t i = 0; i < tiles.size(); i++ ) {
        tiles[i].draw();
    }

}

std::vector<Tile> &Map::getTiles() {
    return tiles;
}


void Map::loadResources() {

    for ( char c = 'A'; c <= 'Z'; c++ ) {
        tilesTexturesMap[c] = LoadTexture( TextFormat( "resources/images/tiles/tile_%c.png", c ) );
    }

    float tileWidth = 32;
    int currentColumn = 0;
    int currentLine = 0;

    char *mapData = LoadFileText( "resources/maps/map1.txt" );

    while ( *mapData != '\0' ) {

        switch ( *mapData ) {
            case 'a':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        GREEN,
                        nullptr
                    )
                );
                break;
            case 'b':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        BLUE,
                        nullptr
                    )
                );
                break;
            case 'c':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        RED,
                        nullptr
                    )
                );
                break;
            case 'd':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
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
                            Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
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