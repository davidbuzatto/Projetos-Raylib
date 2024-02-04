/**
 * @file Map.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Map class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Map.h>
#include <ResourceManager.h>

#include <vector>
#include <string>
#include <iostream>
#include <raylib.h>
#include <Coin.h>
#include <Goomba.h>
#include <Sprite.h>

int Map::tileWidth = 32;

Map::Map() :
    maxWidth( 0 ),
    maxHeight( 0 ),
    playerOffset( 0 ),
    parsed( false ) {
}

Map::~Map() {
}

void Map::draw() {

    int repeats = maxWidth / backgroundTexture.width + 2;
    for ( int i = 0; i <= repeats; i++ ) {
        DrawTexture( 
            backgroundTexture, 
            -backgroundTexture.width + i * backgroundTexture.width - playerOffset * 0.06, 
            0, 
            WHITE );
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

void Map::playMusic() {

    std::map<std::string, Music> musics = ResourceManager::getMusics();
    std::string key(TextFormat( "map%d", mapNumber ));

    if ( !IsMusicStreamPlaying( musics[key] ) ) {
        PlayMusicStream( musics[key] );
    } else {
        UpdateMusicStream( musics[key] );
    }

}

void Map::parseMap( int mapNumber, bool loadTestMap ) {

    if ( !parsed ) {

        this->mapNumber = mapNumber;
        char *mapData;
        
        if ( loadTestMap ) {
            mapData = LoadFileText( TextFormat( "resources/maps/mapTestes.txt" ) );
        } else {
            mapData = LoadFileText( TextFormat( "resources/maps/map%d.txt", mapNumber ) );
        }

        std::map<std::string, Texture2D> &textures = ResourceManager::getTextures();    
        backgroundTexture = textures[TextFormat("background%d", mapNumber)];

        int currentColumn = 0;
        int currentLine = 0;

        while ( *mapData != '\0' ) {

            float x = currentColumn*tileWidth;
            float y = currentLine*tileWidth;

            if ( maxWidth < x ) {
                maxWidth = x;
            }

            if ( maxHeight < y ) {
                maxHeight = y;
            }

            switch ( *mapData ) {
                case 'a':
                    tiles.push_back( 
                        Tile( 
                            Vector2( x, y ), 
                            Vector2( tileWidth, tileWidth ), 
                            GREEN,
                            ""
                        )
                    );
                    break;
                case 'b':
                    tiles.push_back( 
                        Tile( 
                            Vector2( x, y ), 
                            Vector2( tileWidth, tileWidth ), 
                            BLUE,
                            ""
                        )
                    );
                    break;
                case 'c':
                    tiles.push_back( 
                        Tile( 
                            Vector2( x, y ), 
                            Vector2( tileWidth, tileWidth ), 
                            RED,
                            ""
                        )
                    );
                    break;
                case 'd':
                    tiles.push_back( 
                        Tile( 
                            Vector2( x, y ), 
                            Vector2( tileWidth, tileWidth ), 
                            ORANGE,
                            ""
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
                                std::string(1, *mapData)
                            )
                        );
                    }
                    break;
            }

            currentColumn++;
            mapData++;
            
        }

        maxHeight += tileWidth;
        parsed = true;

    }

}

float Map::getMaxWidth() {
    return maxWidth;
}

float Map::getMaxHeight() {
    return maxHeight;
}

void Map::setPlayerOffset( float playerOffset ) {
    this->playerOffset = playerOffset;
}