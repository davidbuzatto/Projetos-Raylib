/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWorld.h>

#include <iostream>
#include <fmt/format.h>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <vector>
#include <raylib.h>
//#include <raymath.h>
//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>
//#undef RAYGUI_IMPLEMENTATION

#include <Player.h>
#include <Tile.h>

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() :
    player( 
        Vector2( 96, 100 ), 
        Vector2( 0, 0 ), 
        Vector2( 28, 40 ), 
        Color( 0, 0, 0, 255 ),
        260,
        360,
        -550,
        20
    ),
    camera( nullptr ) {
    std::cout << "creating game world..." << std::endl;
    debug = true;
    map.setDebug( debug );
    player.setDebug( debug );
}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {

    player.update();

    // player x tiles collision resolution
    std::vector<Tile> &tiles = map.getTiles();
    for ( size_t i = 0; i < tiles.size(); i++ ) {
        player.checkCollision( tiles[i] );
    }

    camera->offset.x = GetScreenWidth()/2.0;
    camera->offset.y = GetScreenHeight() - (64 + player.getHeight()/2);
    camera->target.x = player.getX() + player.getWidth()/2;
    camera->target.y = player.getY() + player.getHeight()/2;

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() {

    BeginDrawing();
    ClearBackground( WHITE );

    int tileWidth = 32;
    int columns = GetScreenWidth() / tileWidth;
    int lines = GetScreenHeight() / tileWidth;

    BeginMode2D( *camera );

    map.draw();
    player.draw();

    if ( debug ) {
        for ( int i = -20; i <= lines + 20; i++ ) {
            DrawLine( -2000, i*tileWidth, 10000, i*tileWidth, GRAY );
        }
        for ( int i = -20; i <= columns + 250; i++ ) {
            DrawLine( i*tileWidth, -2000, i*tileWidth, 2000, GRAY );
        }
    }

    EndMode2D();

    if ( debug ) {
        DrawFPS( 20, 20 );
    }

    EndDrawing();

}

/**
 * @brief Load game resources like images, textures, sounds, fonts, shaders etc.
 * Should be called inside the constructor.
 */
void GameWorld::loadResources() {
    std::cout << "loading resources..." << std::endl;
    player.loadResources();
    map.loadResources();
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources() {
    std::cout << "unloading resources..." << std::endl;
}

void GameWorld::setCamera( Camera2D *camera ) {
    this->camera = camera;
}