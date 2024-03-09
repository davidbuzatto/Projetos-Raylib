/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Main function. Base template for game development in C++ using
 * Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include "GameWindow.h"

int main( void ) {

    GameWindow gameWindow( 
        800,             // height
        450,             // width   
        "Window Title",  // title
        60,              // target FPS
        true,            // antialiazing
        false,           // resizable
        false,           // full screen
        false,           // undecorated
        false,           // always on top
        false,           // always run
        false            // init audio
    );
    
    gameWindow.init();

    return 0;

}