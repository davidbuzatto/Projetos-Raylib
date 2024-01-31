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

    float tileWidth = 32;
    int currentColumn = 0;
    int currentLine = 0;

    std::string mapData = 
R"(a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a                       a
a           cc          a
a           cc          a
a          bbbb         a
a         bbbbbb        a
aaaaaaaaaaaaaaaaaaaaaaaaa)";

    for ( size_t i = 0; i < mapData.length(); i++ ) {
        switch ( mapData[i] ) {
            case 'a':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        Color( 0, 200, 0, 255 )
                    )
                );
                currentColumn++;
                break;
            case 'b':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        Color( 0, 0, 200, 255 )
                    )
                );
                currentColumn++;
                break;
            case 'c':
                tiles.push_back( 
                    Tile( 
                        Vector2( currentColumn*tileWidth, currentLine*tileWidth ), 
                        Vector2( tileWidth, tileWidth ), 
                        Color( 200, 0, 0, 255 )
                    )
                );
                currentColumn++;
                break;
            case ' ':
                currentColumn++;
                break;
            case '\n':
                std::cout << "a" << std::endl;
                currentLine++;
                currentColumn = 0;
                break;
        }
    }
}

Map::~Map() {
}

void Map::draw() const {

    for ( size_t i = 0; i < tiles.size(); i++ ) {
        tiles[i].draw();
    }

}