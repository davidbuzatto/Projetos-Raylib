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
#include <map>
#include <string>
#include <vector>
#include <ResourceManager.h>
#include <raylib.h>
//#include <raymath.h>
//#define RAYGUI_IMPLEMENTATION
//#include <raygui.h>
//#undef RAYGUI_IMPLEMENTATION

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() {
    std::cout << "creating game world..." << std::endl;
}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() {
    std::cout << "destroying game world..." << std::endl;
}

bool playingMusic = false;

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {
    
    std::map<std::string, Sound> sounds = ResourceManager::getSounds();
    std::map<std::string, Music> musics = ResourceManager::getMusics();
    std::map<std::string, Texture2D> textures = ResourceManager::getTextures();

    if ( !playingMusic ) {
        //textures["a"] = LoadTexture( "resources/images/mario.png" );
        playingMusic = true;
        PlayMusicStream( musics[ "overworld1" ] );
    } else {
        UpdateMusicStream( musics[ "overworld1" ] );
        DrawText( TextFormat( "%f", GetMusicTimePlayed( musics[ "overworld1" ] ) ), 100, 100, 20, BLACK );
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
        PlaySound( sounds["powerUp"] );
    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() const {

    BeginDrawing();
    ClearBackground( WHITE );

    std::map<std::string, Texture2D> textures = ResourceManager::getTextures();
    
    DrawTexture( textures["marioSpriteMap"], 0, 0, WHITE );

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