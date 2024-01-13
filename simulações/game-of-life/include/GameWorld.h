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
#include <Drawable.h>
#include <GameState.h>

class GameWorld : public virtual Drawable {

    int cellWidth;
    int lines;
    int columns;

    int *evolutionArray;
    int *resetArray;
    int *newGeneration;
    int evolutionArraySize;

    float time;
    GameState state;

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
    void inputAndUpdate();

    /**
     * @brief Draws the state of the game.
     */
    virtual void draw() const;

    int getCellWidth() const;
    int getColumns() const;
    int getLines() const;

private:

    void createNewGeneration();
    int countNeighbors( int line, int column );

    /**
     * @brief Load game resources like images, textures, sounds, fonts, shaders,
     * etc.
     * Should be called inside the constructor.
     */
    void loadResources();

    /**
     * @brief Unload the once loaded game resources.
     * Should be called inside the destructor.
     */
    void unloadResources();
    
};