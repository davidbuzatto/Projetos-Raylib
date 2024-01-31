/**
 * @file Player.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Player class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Player.h>
#include <PlayerState.h>

#include <raylib.h>

Player::Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, float gravity ) :
    Sprite( pos, dim, vel, color ),
    speedX( speedX ),
    maxSpeedX( maxSpeedX),
    jumpSpeed( jumpSpeed ),
    gravity( gravity ) {
    state = PlayerState::ON_GROUND;
}

Player::~Player() {
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

    // collision resolution...
    if ( pos.y >= 576 ) {
        pos.y = 576;
        state = PlayerState::ON_GROUND;
    }

    vel.y += gravity;

}

void Player::draw() const {
    DrawRectangle( pos.x, pos.y, dim.x, dim.y, color );
}

bool Player::checkCollision( Sprite &sprite ) const {
    return false;
}
