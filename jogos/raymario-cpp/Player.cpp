/**
 * @file Player.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Player class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Player.h>
#include <PlayerState.h>
#include <Direction.h>
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
    facingDirection = Direction::RIGHT;
    crouched = false;

    frameTimeWalking = 0.1;
    frameTimeRunning = 0.05;
    frameAcum = 0;
    currentFrame = 0;
    maxFrames = 2;

    cpN.setColor( PINK );
    cpS.setColor( VIOLET );
    cpE.setColor( YELLOW );
    cpW.setColor( LIME );

    debug = false;

    updateCollisionProbes();
    
}

Player::~Player() {
    unloadResources();
}

void Player::update() {
    
    float delta = GetFrameTime();
    float currentSpeedX = IsKeyDown( KEY_LEFT_CONTROL ) ? maxSpeedX : speedX;
    float currentFrameTime = IsKeyDown( KEY_LEFT_CONTROL ) ? frameTimeRunning : frameTimeWalking;

    if ( vel.x != 0 ) {
        frameAcum += delta;
        if ( frameAcum >= currentFrameTime ) {
            frameAcum = 0;
            currentFrame++;
            currentFrame %= maxFrames;
        }
    } else {
        currentFrame = 0;
    }

    if ( IsKeyDown( KEY_RIGHT ) ) {
        facingDirection = Direction::RIGHT;
        vel.x = currentSpeedX;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        facingDirection = Direction::LEFT;
        vel.x = -currentSpeedX;
    } else {
        vel.x = 0;
    }

    if ( IsKeyDown( KEY_DOWN ) ) {
        crouched = true;
    } else {
        crouched = false;
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

    if ( facingDirection == Direction::RIGHT ) {
        if ( state == PlayerState::ON_GROUND ) {
            if ( crouched ) {
                DrawTexture( textureDR, pos.x, pos.y, WHITE );
            } else {
                if ( currentFrame == 0 ) {
                    DrawTexture( texture1R, pos.x, pos.y, WHITE );
                } else if ( currentFrame == 1 ) {
                    DrawTexture( texture2R, pos.x, pos.y, WHITE );
                }
            }
        } else if ( state == PlayerState::JUMPING ) {
            if ( vel.y < 0 ) {
                DrawTexture( texture1JR, pos.x, pos.y, WHITE );
            } else {
                DrawTexture( texture2JR, pos.x, pos.y, WHITE );
            }
        } 
    } else {
        if ( state == PlayerState::ON_GROUND ) {
            if ( crouched ) {
                DrawTexture( textureDL, pos.x, pos.y, WHITE );
            } else {
                if ( currentFrame == 0 ) {
                    DrawTexture( texture1L, pos.x, pos.y, WHITE );
                } else if ( currentFrame == 1 ) {
                    DrawTexture( texture2L, pos.x, pos.y, WHITE );
                }
            }
        } else if ( state == PlayerState::JUMPING ) {
            if ( vel.y < 0 ) {
                DrawTexture( texture1JL, pos.x, pos.y, WHITE );
            } else {
                DrawTexture( texture2JL, pos.x, pos.y, WHITE );
            }
        }
    }

    if ( debug ) {
        cpN.draw();
        cpS.draw();
        cpE.draw();
        cpW.draw();
    }

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
            if ( debug ) {
                tile.setColor( cpN.getColor() );
            }
            pos.y = tile.getY() + tile.getHeight();
            vel.y = 0;
            updateCollisionProbes();
            return true;
        } else if ( CheckCollisionRecs( cpSRect, tileRect ) ) {
            if ( debug ) {
                tile.setColor( cpS.getColor() );
            }
            pos.y = tile.getY() - dim.y;
            vel.y = 0;
            updateCollisionProbes();
            state = PlayerState::ON_GROUND;
            return true;
        } else if ( CheckCollisionRecs( cpERect, tileRect ) ) {
            if ( debug ) {
                tile.setColor( cpE.getColor() );
            }
            pos.x = tile.getX() - dim.x;
            vel.x = 0;
            updateCollisionProbes();
            return true;
        } else if ( CheckCollisionRecs( cpWRect, tileRect ) ) {
            if ( debug ) {
                tile.setColor( cpW.getColor() );
            }
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
    texture1R  = LoadTexture( "resources/images/sprites/mario1R.png" );
    texture2R  = LoadTexture( "resources/images/sprites/mario2R.png" );
    texture1L  = LoadTexture( "resources/images/sprites/mario1L.png" );
    texture2L  = LoadTexture( "resources/images/sprites/mario2L.png" );
    texture1JR = LoadTexture( "resources/images/sprites/marioJump1R.png" );
    texture2JR = LoadTexture( "resources/images/sprites/marioJump2R.png" );
    texture1JL = LoadTexture( "resources/images/sprites/marioJump1L.png" );
    texture2JL = LoadTexture( "resources/images/sprites/marioJump2L.png" );
    textureDR  = LoadTexture( "resources/images/sprites/marioDown1R.png" );
    textureDL  = LoadTexture( "resources/images/sprites/marioDown1L.png" );
    texture1Dy  = LoadTexture( "resources/images/sprites/marioDying1.png" );
    texture2Dy  = LoadTexture( "resources/images/sprites/marioDying2.png" );
}

void Player::unloadResources() {
    UnloadTexture( texture1R );
    UnloadTexture( texture2R );
    UnloadTexture( texture1L );
    UnloadTexture( texture2L );
    UnloadTexture( texture1JR );
    UnloadTexture( texture2JR );
    UnloadTexture( texture1JL );
    UnloadTexture( texture2JL );
    UnloadTexture( textureDR );
    UnloadTexture( textureDL );
    UnloadTexture( texture1Dy );
    UnloadTexture( texture2Dy );
}

void Player::setDebug( bool debug ) {
    this->debug = debug;
}