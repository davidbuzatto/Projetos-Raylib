/**
 * @file Map.h
 * @author Prof. Dr. David Buzatto
 * @brief Map class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <vector>
#include <raylib.h>
#include <Drawable.h>
#include <Tile.h>

class Map : public virtual Drawable {

protected:
    std::vector<Tile> tiles;

public:
    Map();
    ~Map();
    virtual void draw() const;

};