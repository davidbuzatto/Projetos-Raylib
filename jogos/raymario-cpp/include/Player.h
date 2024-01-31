/**
 * @file Player.h
 * @author Prof. Dr. David Buzatto
 * @brief Player class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <raylib.h>
#include <PlayerState.h>
#include <Sprite.h>

class Player : public virtual Sprite {

    float speedX;
    float maxSpeedX;
    float jumpSpeed;
    float gravity;
    PlayerState state;
    
public:
    Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, float gravity );
    ~Player();
    virtual void update();
    virtual void draw() const;
    virtual bool checkCollision( Sprite &sprite ) const;

};