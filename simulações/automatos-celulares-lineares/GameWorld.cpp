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
GameWorld::GameWorld() : 
        cellWidth( 2 ),
        generationLength( 600 ),
        generations( 600 ),
        rule( 110 ),
        randomizeFirstGeneration( false ) {

    loadResources();
    std::cout << "creating game world..." << std::endl;

    if ( generationLength % 2 == 0 ) {
        generationLength++;
    }

    evolutionArraySize = generationLength * generations;
    evolutionArray = new int[evolutionArraySize];    

    updateRuleValues();
    updateEvolutionArray();

}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
    delete[] evolutionArray;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {

    if ( IsKeyPressed( KEY_UP ) ) {
        rule++;
        rule %= 256;
        updateRuleValues();
        updateEvolutionArray();
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        rule--;
        rule %= 256;
        if ( rule < 0 ) {
            rule = 0;
        }
        updateRuleValues();
        updateEvolutionArray();
    }

    if ( IsKeyPressed( KEY_R ) ) {
        randomizeFirstGeneration = !randomizeFirstGeneration;
        updateEvolutionArray();
    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() const {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < generations; i++ ) {
        for ( int j = 0; j < generationLength; j++ ) {
            if ( evolutionArray[i*generationLength + j] ) {
                DrawRectangle( j * cellWidth, i * cellWidth, cellWidth, cellWidth, BLACK );
            }
        }
    }

    char ruleText[30];
    int ruleTextWidth;
    sprintf( ruleText, "Rule: %d%s", rule, randomizeFirstGeneration ? " (aleatório)" : "" );
    ruleTextWidth = MeasureText( ruleText, 20 );
    Rectangle r = {
        .x = 10,
        .y = 14,
        .width = (float) ( ruleTextWidth + 20 ),
        .height = 30
    };
    DrawRectangleRounded( r, 0.5, 10, Fade( WHITE, 0.9 ) );
    DrawText( ruleText, 21, 22, 20, BLACK );

    EndDrawing();

}

int GameWorld::getCellWidth() const {
    return cellWidth;
}

int GameWorld::getGenerationLength() const {
    return generationLength;
}

int GameWorld::getGenerations() const {
    return generations;
}

void GameWorld::updateRuleValues() {

    std::fill_n( ruleValues, 8, 0 );

    int v = rule;
    int k = 7;

    while ( v != 0 && k >= 0 ) {
        ruleValues[k--] = v % 2;
        v /= 2;
    }

}

void GameWorld::updateEvolutionArray() {

    std::fill_n( evolutionArray, evolutionArraySize, 0 );

    if ( randomizeFirstGeneration ) {
        for ( int i = 0; i < generationLength; i++ ) {
            evolutionArray[i] = GetRandomValue( 0, 1 );
        }
    } else {
        evolutionArray[generationLength/2] = 1;
    }
    
    for ( int i = 1; i < generations; i++ ) {
        for ( int j = 0; j < generationLength; j++ ) {
            int pa = (i-1) * generationLength + (j - 1);
            int pb = (i-1) * generationLength + j;
            int pc = (i-1) * generationLength + (j + 1);
            int a = j-1 < 0 ? 0 : evolutionArray[pa];
            int b = evolutionArray[pb];
            int c = j+1 >= generationLength ? 0 : evolutionArray[pc];
            evolutionArray[i * generationLength + j] = getRuleValue( a, b, c );
        }
    }

}

int GameWorld::getRuleValue( int a, int b, int c ) {
    return ruleValues[7-(a*4+b*2+c)];
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