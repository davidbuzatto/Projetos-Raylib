/**
 * @file GameWindow.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWindow class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWindow.h>

#include <iostream>
#include <raylib.h>

/**
 * @brief Construct a new GameWindow object
 */
GameWindow::GameWindow() : 
        windowTitle( "Formiga da Langton" ) {

    std::cout << "creating game window..." << std::endl;
    
    screenWidth = gw.getBoardWidth();
    screenHeight = gw.getBoardWidth();

}

/**
 * @brief Destroy the GameWindow object
 */
GameWindow::~GameWindow() {
    std::cout << "destroying game window..." << std::endl;
}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished too.
 */
void GameWindow::init() {

    std::cout << "initializing GUI..." << std::endl;
    
    InitWindow( screenWidth, screenHeight, windowTitle.c_str() );
    SetTargetFPS( 60 ); 

    while ( !WindowShouldClose() ) {
        gw.inputAndUpdate();
        gw.draw();
    }

    CloseWindow();

}