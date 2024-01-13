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
        minCellWidth( 1 ),
        boardWidth( 960 ),
        state( GameState::IDLE ) {

    loadResources();
    std::cout << "creating game world..." << std::endl;

    lines = boardWidth / minCellWidth;
    columns = lines;

    cellWidth = allowedCellWidths[currentZoom];
    
    evolutionArraySize = columns * lines;
    evolutionArray = new int[evolutionArraySize];
    resetArray = new int[evolutionArraySize];
    newGeneration = new int[evolutionArraySize];

    drawGrid = true;

    currentTime = 0;
    timeToWait = 0.3;

    std::fill_n( evolutionArray, evolutionArraySize, 0 );

    evolutionArray[454554] = 1;
    evolutionArray[455513] = 1;
    evolutionArray[456473] = 1;
    evolutionArray[456474] = 1;
    evolutionArray[456475] = 1;
    evolutionArray[455525] = 1;
    evolutionArray[454564] = 1;
    evolutionArray[454563] = 1;
    evolutionArray[455523] = 1;
    evolutionArray[456483] = 1;
    evolutionArray[466084] = 1;
    evolutionArray[465125] = 1;
    evolutionArray[464165] = 1;
    evolutionArray[464164] = 1;
    evolutionArray[464163] = 1;
    evolutionArray[465113] = 1;
    evolutionArray[466074] = 1;
    evolutionArray[466075] = 1;
    evolutionArray[465115] = 1;
    evolutionArray[464155] = 1;

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

    int mw = GetMouseWheelMove();
    if ( mw > 0 ) {
        currentZoom++;
        if ( currentZoom > MAX_ZOOM ) {
            currentZoom = MAX_ZOOM;
        }
    } else if ( mw < 0 ) {
        currentZoom--;
        if ( currentZoom < 0 ) {
            currentZoom = 0;
        }
    }
    cellWidth = allowedCellWidths[currentZoom];

    startLine = (lines / 2) - (boardWidth / cellWidth / 2);
    endLine = startLine + (boardWidth / cellWidth);
    startColumn = (columns / 2) - (boardWidth / cellWidth / 2);
    endColumn = startColumn + (boardWidth / cellWidth);

    if ( IsKeyPressed( KEY_UP ) ) {
        timeToWait += 0.05;
        if ( timeToWait > 2.0 ) {
            timeToWait = 2.0;
        }
    } else if ( IsKeyPressed( KEY_DOWN ) ) {
        timeToWait -= 0.05;
        if ( timeToWait < 0.05 ) {
            timeToWait = 0.05;
        }
    }

    if ( state == GameState::RUNNING && currentTime >= timeToWait ) {
        createNewGeneration();
        currentTime = 0;
    } else {
        currentTime += GetFrameTime();
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && state != GameState::RUNNING ) {
        int line = GetMouseY() / cellWidth + startLine;
        int column = GetMouseX() / cellWidth + startColumn;
        if ( evolutionArray[line*columns + column] == 0 ) {
            evolutionArray[line*columns + column] = 1;
            //std::cout << "added: " << line*columns+column << std::endl;
        }
    } else if ( IsMouseButtonDown( MOUSE_BUTTON_RIGHT ) && state != GameState::RUNNING ) {
        int line = GetMouseY() / cellWidth + startLine;
        int column = GetMouseX() / cellWidth + startColumn;
        if ( evolutionArray[line*columns + column] == 1 ) {
            evolutionArray[line*columns + column] = 0;
            //std::cout << "removed: " << line*columns+column << std::endl;
        }
    }

    if ( IsKeyPressed( KEY_R ) && state != GameState::IDLE ) {
        std::copy( resetArray, resetArray+evolutionArraySize, evolutionArray );
        state = GameState::IDLE;
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        if ( state == GameState::IDLE ) {
            std::copy( evolutionArray, evolutionArray+evolutionArraySize, resetArray );
            state = GameState::RUNNING;
        } else if ( state == GameState::RUNNING ) {
            state = GameState::PAUSED;
        } else if ( state == GameState::PAUSED ) {
            state = GameState::RUNNING;
        }
    }

    if ( IsKeyPressed( KEY_G ) ) {
        drawGrid = !drawGrid;
    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() const {

    BeginDrawing();
    ClearBackground( WHITE );

    for ( int i = startLine; i < endLine; i++ ) {
        for ( int j = startColumn; j <= endColumn; j++ ) {
            if ( evolutionArray[i*columns + j] ) {
                DrawRectangle( 
                    j * cellWidth - startColumn * cellWidth, 
                    i * cellWidth - startLine * cellWidth, 
                    cellWidth, cellWidth, BLACK );
            }
        }
    }

    if ( drawGrid ) {
        for ( int i = 1; i < endLine - startLine; i++ ) {
            if ( i % 10 == 0 ) {
                DrawLine( 0, i * cellWidth, GetScreenWidth(), i * cellWidth, BLACK );
            } else {
                DrawLine( 0, i * cellWidth, GetScreenWidth(), i * cellWidth, GRAY );
            }
        }

        for ( int i = 1; i < endColumn - startColumn; i++ ) {
            if ( i % 10 == 0 ) {
                DrawLine( i * cellWidth, 0, i * cellWidth, GetScreenHeight(), BLACK );
            } else {
                DrawLine( i * cellWidth, 0, i * cellWidth, GetScreenHeight(), GRAY );
            }
        }
    }

    DrawText( TextFormat( "%.2f segundos para a próxima geração.", timeToWait ), 20, 20, 20, BLUE );

    EndDrawing();

}

int GameWorld::getBoardWidth() const {
    return boardWidth;
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