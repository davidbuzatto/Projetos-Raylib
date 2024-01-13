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

class GameWorld : public virtual Drawable {

    int cellWidth;
    int generationLength;
    int generations;
    int rule;
    bool randomizeFirstGeneration;

    int *evolutionArray;
    int evolutionArraySize;

    int ruleValues[8];

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
    int getGenerationLength() const;
    int getGenerations() const;

private:

    void updateRuleValues();
    void updateEvolutionArray();
    int getRuleValue( int a, int b, int c );

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