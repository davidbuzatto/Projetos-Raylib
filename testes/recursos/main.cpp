/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Main function. Base template for game development in C++ using
 * Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWindow.h>

int main( void ) {

    GameWindow gameWindow( 800, 640, "Testes recursos", true );
    gameWindow.init();

    return 0;

}