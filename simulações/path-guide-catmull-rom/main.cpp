/**
 * @file main.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Path with Catmull-Rom guide. The code is a mess...
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
    const int screenHeight = 800;
    
    SetConfigFlags( FLAG_MSAA_4X_HINT );
    InitWindow( screenWidth, screenHeight, "Caminho com Guias Catmull-Rom" );
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
