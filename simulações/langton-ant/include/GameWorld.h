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

    int minCellWidth;
    int boardWidth;
    int lines;
    int columns;

    int *evolutionArray;
    int *resetArray;
    int *newGeneration;
    int evolutionArraySize;

    const int MAX_ZOOM = 6;
    const int allowedCellWidths[8] = { 1, 2, 4, 8, 12, 24, 48 };
    int currentZoom = 5;
    int cellWidth;

    int startLine;
    int endLine;
    int startColumn;
    int endColumn;

    bool drawGrid;

    float currentTime;
    float timeToWait;
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

    int getBoardWidth() const;

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