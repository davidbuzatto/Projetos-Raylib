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
#include <CollisionProbe.h>
#include <Sprite.h>

class Player : public virtual Sprite {

    float speedX;
    float maxSpeedX;
    float jumpSpeed;
    float gravity;
    PlayerState state;
    Texture2D texture;

    CollisionProbe cpN;
    CollisionProbe cpS;
    CollisionProbe cpE;
    CollisionProbe cpW;
    CollisionProbe cpNE;
    CollisionProbe cpSE;
    CollisionProbe cpSW;
    CollisionProbe cpNW;
    
public:
    Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, float gravity );
    ~Player();
    virtual void update();
    virtual void draw();
    virtual bool checkCollision( Sprite &sprite );
    void setState( PlayerState state );
    PlayerState getState() const;
    void updateCollisionProbes();

    float getSpeedX();
    float getMaxSpeedX();
    float getJumpSpeed();

    void loadResources();
    void unloadResources();

};