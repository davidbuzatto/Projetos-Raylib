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

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld( void ) {
    loadResources();
    std::cout << "creating game world..." << std::endl;
}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld( void ) {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate( void ) {

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw( void ) const {

    BeginDrawing();
    ClearBackground( WHITE );

    const char *text = "Basic game template";
    Vector2 m = MeasureTextEx( GetFontDefault(), text, 40, 4 );
    int x = GetScreenWidth() / 2 - m.x / 2;
    int y = GetScreenHeight() / 2 - m.y / 2;
    DrawRectangle( x, y, m.x, m.y, BLACK );
    DrawText( text, x, y, 40, WHITE );

    EndDrawing();

}

/**
 * @brief Load game resources like images, textures, sounds, fonts, shaders etc.
 * Should be called inside the constructor.
 */
void GameWorld::loadResources( void ) {
    std::cout << "loading resources..." << std::endl;
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources( void ) {
    std::cout << "unloading resources..." << std::endl;
}