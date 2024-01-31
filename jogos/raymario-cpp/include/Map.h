/**
 * @file Map.h
 * @author Prof. Dr. David Buzatto
 * @brief Map class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <vector>
#include <map>
#include <raylib.h>
#include <Drawable.h>
#include <Tile.h>

class Map : public virtual Drawable {

    std::vector<Tile> tiles;
    std::map<char, Texture2D> tilesTexturesMap;

public:
    Map();
    ~Map();
    virtual void draw();
    std::vector<Tile> &getTiles();
    void loadResources();
    void unloadResources();

};