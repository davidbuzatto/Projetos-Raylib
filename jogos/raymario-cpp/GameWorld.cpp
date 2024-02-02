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
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>
#undef RAYGUI_IMPLEMENTATION

#include <Player.h>
#include <Tile.h>

bool GameWorld::debug = true;
float GameWorld::gravity = 20;

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
        -550
    ),
    camera( nullptr ) {
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

    std::vector<Goomba> &goombas = map.getGoombas();

    player.update();
    for ( size_t i = 0; i < goombas.size(); i++ ) {
        goombas[i].update();
    }

    // player x tiles collision resolution
    std::vector<Tile> &tiles = map.getTiles();
    for ( size_t i = 0; i < tiles.size(); i++ ) {
        player.checkCollision( tiles[i] );
    }

    // goombas x tiles collision resolution
    for ( size_t i = 0; i < tiles.size(); i++ ) {
        for ( size_t j = 0; j < goombas.size(); j++ ) {
            goombas[j].checkCollision( tiles[i] );
        }
    }

    // player x coins collision resolution
    std::vector<Coin> &coins = map.getCoins();
    std::vector<int> collectCoins;
    for ( size_t i = 0; i < coins.size(); i++ ) {
        if ( coins[i].checkCollision( player ) ) {
            collectCoins.push_back(i);
        }
    }
    for ( int i = collectCoins.size() - 1; i >= 0; i-- ) {
        coins.erase( coins.begin() + collectCoins[i] );
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
        DrawFPS( 30, 90 );
    }

    GuiPanel( Rectangle( 20, 20, 100, 60 ), "Controles" );
    if ( GuiButton( Rectangle( 30, 50, 60, 20 ), "debug" ) == 1 ) {
        debug = !debug;
        if ( !debug ) {
            for ( size_t i = 0; i < map.getTiles().size(); i++ ) {
                map.getTiles()[i].setColor( BLACK );
            }
        }
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