/**
 * @file Coin.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Coin class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Coin.h>
#include <Player.h>
#include <vector>
#include <iostream>
#include <raylib.h>

std::vector<Texture2D> Coin::textures;

Coin::Coin( Vector2 pos, Vector2 dim, Color color ) :
    Sprite( pos, dim, color ) {
    
    frameTime = 0.1;
    frameAcum = 0;
    currentFrame = 0;
    
}

Coin::~Coin() {
}

void Coin::update() {
    
    frameAcum += GetFrameTime();
    if ( frameAcum >= frameTime ) {
        frameAcum = 0;
        currentFrame++;
        currentFrame %= textures.size();
    }

}

void Coin::draw() {

    update();
    DrawTexture( Coin::textures[currentFrame], pos.x, pos.y, WHITE );

}

bool Coin::checkCollision( Sprite &sprite ) {

    try {

        Player &player = dynamic_cast<Player&>(sprite);
        Rectangle coinRect( pos.x, pos.y, dim.x, dim.y );
        Rectangle playerRect( player.getX(), player.getY(), player.getWidth(), player.getHeight() );
        return CheckCollisionRecs( coinRect, playerRect );

    } catch ( std::bad_cast const& ) {
    }

    return false;

}

void Coin::loadResources() {
    textures.push_back( LoadTexture( "resources/images/sprites/coin1.png" ) );
    textures.push_back( LoadTexture( "resources/images/sprites/coin2.png" ) );
    textures.push_back( LoadTexture( "resources/images/sprites/coin3.png" ) );
    textures.push_back( LoadTexture( "resources/images/sprites/coin4.png" ) );
    textures.push_back( LoadTexture( "resources/images/sprites/coin5.png" ) );
}

void Coin::unloadResources() {
    for ( size_t i = 0; i < textures.size(); i++ ) {
        UnloadTexture( textures[i] );
    }
}