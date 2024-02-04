/**
 * @file Tile.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Tile class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Tile.h>
#include <ResourceManager.h>

#include <iostream>
#include <string>
#include <raylib.h>

Tile::Tile( Vector2 pos, Vector2 dim, Color color, std::string key ) :
    Sprite( pos, dim, color ),
    key( key ) {
}

Tile::~Tile() {
}

void Tile::update() {
}

void Tile::draw() {

    std::map<std::string, Texture2D> &textures = ResourceManager::getTextures();

    if ( key.length() != 0 ) {
        DrawTexture( textures[key], pos.x, pos.y, WHITE );
        if ( !(color.r == 0 && color.g == 0 && color.b == 0) ) {
            DrawRectangle( pos.x, pos.y, dim.x, dim.y, Fade( color, 0.5 ) );
        }
    } else {
        DrawRectangle( pos.x, pos.y, dim.x, dim.y, color );
    }
}

bool Tile::checkCollision( Sprite &sprite ) {
    return false;
}
