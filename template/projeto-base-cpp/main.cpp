/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Main function. Base template for game development in C++ using
 * Raylib (https://www.raylib.com/).
 * 
 * @copyright Copyright (c) 2024
 */
#include <iostream>
#include <fmt/format.h>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <raylib.h>

#include <GameWorld.h>

int main( void ) {

    const int screenWidth = 800;
    const int screenHeight = 450;
    
    // turn antialiasing on (if possible)
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Window Title" );
    InitAudioDevice();
    SetTargetFPS( 60 );    

    GameWorld gw;

    while ( !WindowShouldClose() ) {
        gw.inputAndUpdate();
        gw.draw();
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;

}
