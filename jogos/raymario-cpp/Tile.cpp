/**
 * @file Tile.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Tile class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Tile.h>

#include <iostream>
#include <raylib.h>

Tile::Tile( Vector2 pos, Vector2 dim, Color color, Texture2D *texture ) :
    Sprite( pos, dim, color ),
    texture( texture ) {
}

Tile::~Tile() {
}

void Tile::update() {
}

void Tile::draw() {
    if ( texture != nullptr ) {
        DrawTexture( *texture, pos.x, pos.y, WHITE );
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
