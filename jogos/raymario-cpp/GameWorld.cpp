/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWorld.h>
#include <ResourceManager.h>

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
    std::cout << "destroying game world..." << std::endl;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {

    map.parseMap( 1, false );
    map.playMusic();

    std::vector<Goomba> &goombas = map.getGoombas();
    std::map<std::string, Sound> &sounds = ResourceManager::getSounds();

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
            PlaySound( sounds[ "coin" ] );
        }
    }
    for ( int i = collectCoins.size() - 1; i >= 0; i-- ) {
        coins.erase( coins.begin() + collectCoins[i] );
    }

    if ( IsGamepadButtonPressed( 0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1 ) ) {
        debug = !debug;
        if ( !debug ) {
            for ( size_t i = 0; i < map.getTiles().size(); i++ ) {
                map.getTiles()[i].setColor( BLACK );
            }
        }
    }

    float xc = GetScreenWidth() / 2.0;
    float yc = GetScreenHeight() / 2.0;
    float pxc = player.getX() + player.getWidth() / 2.0;
    float pyc = player.getY() + player.getHeight() / 2.0;
    
    camera->offset.x = xc;

    if ( pxc < xc ) {
        camera->target.x = xc;
        map.setPlayerOffset( 0 );         // x parallax
    } else if ( pxc >= map.getMaxWidth() - xc ) {
        camera->target.x = map.getMaxWidth() - GetScreenWidth();
        camera->offset.x = 0;
    } else {
        camera->target.x = pxc;
        map.setPlayerOffset( pxc - xc );  // x parallax
    }

    camera->offset.y = yc;

    if ( pyc < yc ) {
        camera->target.y = yc;
    } else if ( pyc >= map.getMaxHeight() - yc ) {
        camera->target.y = map.getMaxHeight() - GetScreenHeight();
        camera->offset.y = 0;
    } else {
        camera->target.y = pyc;
    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() {

    BeginDrawing();
    ClearBackground( WHITE );

    int columns = GetScreenWidth() / Map::tileWidth;
    int lines = GetScreenHeight() / Map::tileWidth;

    BeginMode2D( *camera );

    map.draw();
    player.draw();

    if ( debug ) {
        for ( int i = -20; i <= lines + 20; i++ ) {
            DrawLine( -2000, i*Map::tileWidth, 10000, i*Map::tileWidth, GRAY );
        }
        for ( int i = -20; i <= columns + 250; i++ ) {
            DrawLine( i*Map::tileWidth, -2000, i*Map::tileWidth, 2000, GRAY );
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
    ResourceManager::loadTextures();
    ResourceManager::loadSounds();
    ResourceManager::loadMusics();
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources() {
    std::cout << "unloading resources..." << std::endl;
    ResourceManager::unloadTextures();
    ResourceManager::unloadSounds();
    ResourceManager::unloadMusics();
}

void GameWorld::setCamera( Camera2D *camera ) {
    this->camera = camera;
}