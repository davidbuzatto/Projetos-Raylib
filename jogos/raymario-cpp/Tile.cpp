/**
 * @file Tile.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Tile class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Tile.h>

#include <raylib.h>

Tile::Tile( Vector2 pos, Vector2 dim, Color color ) :
    Sprite( pos, dim, color ) {
}

Tile::~Tile() {
}

void Tile::update() {
}

void Tile::draw() const {
    DrawRectangle( pos.x, pos.y, dim.x, dim.y, color );
}

bool Tile::checkCollision( Sprite &sprite ) const {
    return false;
}
