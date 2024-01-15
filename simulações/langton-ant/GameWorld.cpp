/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWorld.h>

#include <vector>
#include <iostream>
#include <raylib.h>

#include <GameState.h>

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() : 
        minCellWidth( 1 ),
        boardWidth( 960 ),
        state( GameState::IDLE ),
        ant( Ant( 479, 479 ) ),
        antMovesPerStep( 1 ),
        initialColor( 0xFFFFFFFF ),
        showInfo( true ) {

    loadResources();
    std::cout << "creating game world..." << std::endl;

    lines = boardWidth / minCellWidth;
    columns = lines;

    cellWidth = allowedCellWidths[currentZoom];
    ant.setCellWidth( cellWidth );
    
    boardSize = columns * lines;
    board = new unsigned int[boardSize];

    drawGrid = true;

    currentTime = 0;
    timeToWait = 0.5;

    currentMove = 0;

    generateAntDecisions( "RL", 0, 0, 1, 0, initialColor ); // base Langton Ant
    //generateAntDecisions( "RLR", 195, 285, 1, 0.9, initialColor );
    //generateAntDecisions( "LLRR", 195, 285, 1, 0.9, initialColor );
    //generateAntDecisions( "LRRRRRLLR", 45, 255, 1, 0.8, initialColor );
    //generateAntDecisions( "RRLLLRLLLRRR", 0, 60, 1, 0.9, initialColor );
    //generateAntDecisions( "RRLLLRLLLLLLLLL", 195, 285, 0.7, 0.9, initialColor );
    //generateAntDecisions( "RRLLLRLLLLLLLLL", 285, 195, 0.7, 0.9, initialColor );

    std::fill_n( board, boardSize, initialColor );

}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
    delete[] board;
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

    ant.setStartLine( startLine );
    ant.setStartColumn( startColumn );
    ant.setCellWidth( cellWidth );

    if ( IsKeyDown( KEY_UP ) ) {
        timeToWait *= 2;
        if ( timeToWait > 2.0 ) {
            timeToWait = 2.0;
        }
    } else if ( IsKeyDown( KEY_DOWN ) ) {
        timeToWait /= 2;
        if ( timeToWait < 0.001 ) {
            timeToWait = 0.001;
        }
    }

    if ( IsKeyDown( KEY_RIGHT ) ) {
        antMovesPerStep += 10;        
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        antMovesPerStep -= 10;
        if ( antMovesPerStep <= 0 ) {
            antMovesPerStep = 1;
        }
    }

    if ( state == GameState::RUNNING && currentTime >= timeToWait ) {
        nextStep();
        currentTime = 0;
    } else {
        currentTime += GetFrameTime();
    }

    if ( IsKeyPressed( KEY_R ) ) {
        std::fill_n( board, boardSize, initialColor );
        ant.setLine( 479 );
        ant.setColumn( 479 );
        ant.setMoving( false );
        currentMove = 0;
        state = GameState::IDLE;
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        if ( state == GameState::IDLE ) {
            state = GameState::RUNNING;
            ant.setMoving( true );
        } else if ( state == GameState::RUNNING ) {
            state = GameState::PAUSED;
            ant.setMoving( false );
        } else if ( state == GameState::PAUSED ) {
            state = GameState::RUNNING;
            ant.setMoving( true );
        }
    }

    if ( IsKeyPressed( KEY_G ) ) {
        drawGrid = !drawGrid;
    }

    if ( IsKeyPressed( KEY_S ) ) {
        showInfo = !showInfo;
        ant.setDrawDecisionCycle( showInfo );
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
            if ( board[i*columns + j] ) {
                DrawRectangle( 
                    j * cellWidth - startColumn * cellWidth, 
                    i * cellWidth - startLine * cellWidth, 
                    cellWidth, cellWidth, GetColor( board[i*columns + j] ) );
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

    ant.draw();

    if ( showInfo ) {
        DrawRectangle( 10, 10, 600, 75, Fade( WHITE, 0.8 ) );
        DrawRectangleLines( 10, 10, 600, 75, BLACK );
        DrawText( TextFormat( "%.3f segundo(s) para o próximo passo (impreciso).", timeToWait ), 20, 20, 20, BLUE );
        DrawText( TextFormat( "%d movimento(s) por passo.", antMovesPerStep ), 20, 40, 20, BLUE );
        DrawText( TextFormat( "movimento atual: %d", currentMove ), 20, 60, 20, BLUE );
    }

    EndDrawing();

}

int GameWorld::getBoardWidth() const {
    return boardWidth;
}

void GameWorld::nextStep() {
    for ( int i = 0; i < antMovesPerStep; i++ ) {
        if ( ant.isMoving() ) {
            currentMove++;
            ant.move( board, lines, columns );
        }
    }
}

void GameWorld::generateAntDecisions( 
    std::string turns, 
    int startHue, 
    int endHue, 
    float saturation, 
    float value, 
    unsigned int initialColor ) {
    
    unsigned int color;
    size_t steps = turns.size();
    int hueStep = abs( endHue - startHue ) / steps;

    if ( startHue > endHue ) {
        hueStep = -hueStep;
    }

    for ( size_t i = 0; i < steps; i++ ) {
        if ( i == 0 ) {
            color = initialColor;
        } else {
            color = ColorToInt( ColorFromHSV( startHue + (i-1) * hueStep, saturation, value ) );
        }
        ant.addDecision( Decision( color, turns[i] == 'L' ? TurnType::TURN_LEFT : TurnType::TURN_RIGHT ) );
    }

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