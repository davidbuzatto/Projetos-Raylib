/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Main function. Mario in C++ using Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWindow.h>

int main( void ) {

    GameWindow gameWindow( 800, 640, "Ray Mario", false );
    gameWindow.init();

    return 0;

}