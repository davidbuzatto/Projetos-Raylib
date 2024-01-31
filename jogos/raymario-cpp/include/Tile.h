/**
 * @file Tile.h
 * @author Prof. Dr. David Buzatto
 * @brief Tile class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <raylib.h>
#include <Sprite.h>

class Tile : public virtual Sprite {

    Texture2D *texture;

public:
    Tile( Vector2 pos, Vector2 dim, Color color, Texture2D *texture );
    ~Tile();
    virtual void update();
    virtual void draw();
    virtual bool checkCollision( Sprite &sprite );

};