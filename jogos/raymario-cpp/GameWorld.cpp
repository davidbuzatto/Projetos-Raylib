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
#include <raylib.h>
//#include <raymath.h>
//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>
//#undef RAYGUI_IMPLEMENTATION

#include <Player.h>

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() :
    player( 
        Vector2( 96, 576 ), 
        Vector2( 0, 0 ), 
        Vector2( 32, 32 ), 
        Color( 0, 0, 0, 255 ),
        160,
        240,
        -450,
        20
    ) {
    loadResources();
    std::cout << "creating game world..." << std::endl;
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

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() const {

    BeginDrawing();
    ClearBackground( WHITE );

    int tileWidth = 32;
    int columns = GetScreenWidth() / tileWidth;
    int lines = GetScreenHeight() / tileWidth;

    map.draw();
    player.draw();

    for ( int i = 0; i <= lines; i++ ) {
        DrawLine( 0, i*tileWidth, GetScreenWidth(), i*tileWidth, GRAY );
    }

    for ( int i = 0; i <= columns; i++ ) {
        DrawLine( i*tileWidth, 0, i*tileWidth, GetScreenHeight(), GRAY );
    }

    DrawFPS( 40, 10 );

    EndDrawing();

}

/**
 * @brief Load game resources like images, textures, sounds, fonts, shaders etc.
 * Should be called inside the constructor.
 */
void GameWorld::loadResources() {
    std::cout << "loading resources..." << std::endl;
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources() {
    std::cout << "unloading resources..." << std::endl;
}