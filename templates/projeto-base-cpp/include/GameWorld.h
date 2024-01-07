/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class declaration. This class should contain all
 * game components and state.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <raylib.h>

class GameWorld {
    
public:

    /**
     * @brief Construct a new GameWorld object.
     */
    GameWorld();

    /**
     * @brief Destroy the GameWorld object.
     */
    ~GameWorld();

    /**
     * @brief Reads user input and updates the state of the game.
     */
    void inputAndUpdate( void );

    /**
     * @brief Draws the state of the game.
     */
    void draw( void );

private:

    /**
     * @brief Load game resources like images, textures, sounds, fonts, shaders,
     * etc.
     * Should be called inside the constructor.
     */
    void loadResources( void );

    /**
     * @brief Unload the once loaded game resources.
     * Should be called inside the destructor.
     */
    void unloadResources( void );
    
};