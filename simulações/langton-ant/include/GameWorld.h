/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class declaration. This class should contain all
 * game components and state.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <string>
#include <Drawable.h>
#include <GameState.h>
#include <Ant.h>

class GameWorld : public virtual Drawable {

    int minCellWidth;
    int boardWidth;
    int lines;
    int columns;

    unsigned int *board;
    int boardSize;

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

    Ant ant;
    int antMovesPerStep;
    int currentMove;

    unsigned int initialColor;
    bool showInfo;

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

    void nextStep();

    void generateAntDecisions( 
        std::string turns, 
        int startHue, 
        int endHue, 
        float saturation, 
        float value, 
        unsigned int initialColor );

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