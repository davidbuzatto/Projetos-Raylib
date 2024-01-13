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
#include <algorithm>
#include <iterator>
#include <raylib.h>

#include <GameState.h>

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() : 
        cellWidth( 24 ),
        lines( 50 ),
        columns( 50 ),
        state( GameState::IDLE ) {

    loadResources();
    std::cout << "creating game world..." << std::endl;

    evolutionArraySize = columns * lines;
    evolutionArray = new int[evolutionArraySize];
    resetArray = new int[evolutionArraySize];
    newGeneration = new int[evolutionArraySize];

    std::fill_n( evolutionArray, evolutionArraySize, 0 );

    evolutionArray[1024] = 1;
    evolutionArray[1073] = 1;
    evolutionArray[1123] = 1;
    evolutionArray[1124] = 1;
    evolutionArray[1125] = 1;

}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
    delete[] evolutionArray;
    delete[] newGeneration;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {

    if ( state == GameState::RUNNING && time >= 0.3 ) {
        createNewGeneration();
        time = 0;
    } else {
        time += GetFrameTime();
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && state == GameState::IDLE ) {
        int line = GetMouseY() / cellWidth;
        int column = GetMouseX() / cellWidth;
        if ( evolutionArray[line*columns + column] == 0 ) {
            evolutionArray[line*columns + column] = 1;
            std::cout << "added: " << line*columns+column << std::endl;
        }
    } else if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) && state == GameState::IDLE ) {
        int line = GetMouseY() / cellWidth;
        int column = GetMouseX() / cellWidth;
        if ( evolutionArray[line*columns + column] == 1 ) {
            evolutionArray[line*columns + column] = 0;
            std::cout << "removed: " << line*columns+column << std::endl;
        }
    }

    if ( IsKeyPressed( KEY_R ) && state == GameState::RUNNING ) {
        std::copy( resetArray, resetArray+evolutionArraySize, evolutionArray );
        state = GameState::IDLE;
    }

    if ( IsKeyPressed( KEY_SPACE ) && state == GameState::IDLE ) {
        std::copy( evolutionArray, evolutionArray+evolutionArraySize, resetArray );
        state = GameState::RUNNING;
    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() const {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = 0; i < lines; i++ ) {
        for ( int j = 0; j < columns; j++ ) {
            if ( evolutionArray[i*columns + j] ) {
                DrawRectangle( j * cellWidth, i * cellWidth, cellWidth, cellWidth, BLACK );
            }
        }
    }

    for ( int i = 1; i < lines; i++ ) {
        DrawLine( 0, i * cellWidth, GetScreenWidth(), i * cellWidth, GRAY );
    }

    for ( int i = 1; i < columns; i++ ) {
        DrawLine( i * cellWidth, 0, i * cellWidth, GetScreenHeight(), GRAY );
    }

    EndDrawing();

}

int GameWorld::getCellWidth() const {
    return cellWidth;
}

int GameWorld::getLines() const {
    return lines;
}

int GameWorld::getColumns() const {
    return columns;
}

void GameWorld::createNewGeneration() {

    for ( int i = 0; i < lines; i++ ) {
        for ( int j = 0; j < columns; j++ ) {
            int p = i*columns+j;
            int n = countNeighbors( i, j );
            if ( evolutionArray[p] ) {
                if ( n <= 1 || n >= 4 ) {
                    newGeneration[p] = 0;
                } else {
                    newGeneration[p] = 1;
                }
            } else {
                if ( n == 3 ) {
                    newGeneration[p] = 1;
                } else {
                    newGeneration[p] = 0;
                }
            }
        }
    }

    std::copy( newGeneration, newGeneration+evolutionArraySize, evolutionArray );

}

int GameWorld::countNeighbors( int line, int column ) {

    int count = 0;

    for ( int i = line-1; i < line + 2; i++ ) {
        for ( int j = column-1; j < column + 2; j++ ) {
            if ( i >= 0 &&
                 i < lines && 
                 j >= 0 &&
                 j < columns &&
                 evolutionArray[i*columns+j] ) {
                count++;
            }
        }
    }

    if ( evolutionArray[line*columns+column] ) {
        count--;
    }

    return count;

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