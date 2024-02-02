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
#include <Coin.h>
#include <Tile.h>
#include <Sprite.h>
#include <Goomba.h>

class Map : public virtual Drawable {

    std::vector<Tile> tiles;
    std::map<char, Texture2D> tilesTexturesMap;
    Texture2D backgroundTexture;
    float maxWidth;

    std::vector<Coin> coins;
    std::vector<Goomba> goombas;

public:
    Map();
    ~Map();
    virtual void draw();
    std::vector<Tile> &getTiles();
    std::vector<Coin> &getCoins();
    std::vector<Goomba> &getGoombas();
    void loadResources();
    void unloadResources();

};