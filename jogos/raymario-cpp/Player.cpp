/**
 * @file Player.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Player class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Player.h>
#include <PlayerState.h>
#include <Tile.h>
#include <typeinfo>
#include <iostream>
#include <raylib.h>

Player::Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, float gravity ) :
    Sprite( pos, dim, vel, color ),
    speedX( speedX ),
    maxSpeedX( maxSpeedX),
    jumpSpeed( jumpSpeed ),
    gravity( gravity ) {
    state = PlayerState::ON_GROUND;
    updateCollisionProbes();

    cpN.setColor( PINK );
    cpS.setColor( VIOLET );
    cpE.setColor( YELLOW );
    cpW.setColor( LIME );
    
}

Player::~Player() {
    unloadResources();
}

void Player::update() {
    
    float delta = GetFrameTime();
    float currentSpeedX = IsKeyDown( KEY_LEFT_CONTROL ) ? maxSpeedX : speedX;

    if ( IsKeyDown( KEY_RIGHT ) ) {
        vel.x = currentSpeedX;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        vel.x = -currentSpeedX;
    } else {
        vel.x = 0;
    }

    if ( IsKeyPressed( KEY_SPACE ) && state != PlayerState::JUMPING ) {
        vel.y = jumpSpeed;
        state = PlayerState::JUMPING;
    }

    pos.x = pos.x + vel.x * delta;
    pos.y = pos.y + vel.y * delta;

    vel.y += gravity;

}

void Player::draw() {
    //DrawRectangle( pos.x, pos.y, dim.x, dim.y, color );
    DrawTexture( texture, pos.x, pos.y, WHITE );
    /*cpN.draw();
    cpS.draw();
    cpE.draw();
    cpW.draw();*/
}

bool Player::checkCollision( Sprite &sprite ) {

    try {

        updateCollisionProbes();
        Tile &tile = dynamic_cast<Tile&>(sprite);
        Rectangle tileRect( tile.getX(), tile.getY(), tile.getWidth(), tile.getHeight() );

        Rectangle cpNRect( cpN.getX(), cpN.getY(), cpN.getWidth(), cpN.getHeight() );
        Rectangle cpSRect( cpS.getX(), cpS.getY(), cpS.getWidth(), cpS.getHeight() );
        Rectangle cpERect( cpE.getX(), cpE.getY(), cpE.getWidth(), cpE.getHeight() );
        Rectangle cpWRect( cpW.getX(), cpW.getY(), cpW.getWidth(), cpW.getHeight() );

        if ( CheckCollisionRecs( cpNRect, tileRect ) ) {
            tile.setColor( cpN.getColor() );
            pos.y = tile.getY() + tile.getHeight();
            vel.y = 0;
            updateCollisionProbes();
            return true;
        } else if ( CheckCollisionRecs( cpSRect, tileRect ) ) {
            tile.setColor( cpS.getColor() );
            pos.y = tile.getY() - dim.y;
            vel.y = 0;
            updateCollisionProbes();
            state = PlayerState::ON_GROUND;
            return true;
        } else if ( CheckCollisionRecs( cpERect, tileRect ) ) {
            tile.setColor( cpE.getColor() );
            pos.x = tile.getX() - dim.x;
            vel.x = 0;
            updateCollisionProbes();
            return true;
        } else if ( CheckCollisionRecs( cpWRect, tileRect ) ) {
            tile.setColor( cpW.getColor() );
            pos.x = tile.getX() + tile.getWidth();
            vel.x = 0;
            updateCollisionProbes();
            return true;
        }

    } catch ( std::bad_cast const& ) {
    }

    return false;

}

void Player::setState( PlayerState state ) {
    this->state = state;
}

PlayerState Player::getState() const {
    return state;
}

void Player::updateCollisionProbes() {

    cpN.setX( pos.x + dim.x / 2 - cpN.getWidth() / 2 );
    cpN.setY( pos.y );

    cpS.setX( pos.x + dim.x / 2 - cpS.getWidth() / 2 );
    cpS.setY( pos.y + dim.y - cpS.getHeight() );

    cpE.setX( pos.x + dim.x - cpE.getWidth() );
    cpE.setY( pos.y + dim.y / 2 - cpE.getHeight() / 2 );

    cpW.setX( pos.x );
    cpW.setY( pos.y + dim.y / 2 - cpW.getHeight() / 2 );

}

float Player::getSpeedX() {
    return speedX;
}

float Player::getMaxSpeedX() {
    return maxSpeedX;
}

float Player::getJumpSpeed() {
    return jumpSpeed;
}

void Player::loadResources() {
    texture = LoadTexture( "resources/images/sprites/mario1R.png" );
}

void Player::unloadResources() {
    UnloadTexture( texture );
}