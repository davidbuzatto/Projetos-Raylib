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
#include <Direction.h>

class Player : public virtual Sprite {

    float speedX;
    float maxSpeedX;
    float jumpSpeed;

    PlayerState state;
    Direction facingDirection;
    bool crouched;
    
    Texture2D texture1R;
    Texture2D texture2R;
    Texture2D texture1L;
    Texture2D texture2L;
    Texture2D texture1JR;
    Texture2D texture2JR;
    Texture2D texture1JL;
    Texture2D texture2JL;
    Texture2D textureDR;
    Texture2D textureDL;
    Texture2D texture1Dy;
    Texture2D texture2Dy;

    float frameTimeWalking;
    float frameTimeRunning;
    float frameAcum;
    int currentFrame;
    int maxFrames;

    CollisionProbe cpN;
    CollisionProbe cpS;
    CollisionProbe cpE;
    CollisionProbe cpW;
    
public:
    Player( Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed );
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