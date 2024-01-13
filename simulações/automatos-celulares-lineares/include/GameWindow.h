/**
 * @file GameWindow.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWindow class declaration. This class contains one instance of
 * the game World and it is responsible to create a new Raylib window,
 * start the game loop and finish the Raylib window.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <string>

#include <raylib.h>
#include <GameWorld.h>

class GameWindow {
    
    int screenWidth;
    int screenHeight;
    std::string windowTitle;

    GameWorld gw;

public:

    /**
     * @brief Construct a new GameWindow object.
     */
    GameWindow();

    /**
     * @brief Destroy the GameWindow object.
     */
    ~GameWindow();

    /**
     * @brief Initializes the Window, starts the game loop and, when it
     * finishes, the window will be finished too.
     */
    void init();
    
};