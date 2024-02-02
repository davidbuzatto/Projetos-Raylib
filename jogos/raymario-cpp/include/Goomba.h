/**
 * @file Goomba.h
 * @author Prof. Dr. David Buzatto
 * @brief Goomba class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <vector>
#include <raylib.h>
#include <Sprite.h>
#include <Direction.h>
#include <CollisionProbe.h>

class Goomba : public virtual Sprite {

    float frameTime;
    float frameAcum;
    int currentFrame;
    int maxFrames;
    Direction facingDirection;

    CollisionProbe cpN;
    CollisionProbe cpS;
    CollisionProbe cpE;
    CollisionProbe cpW;
    
public:
    static std::vector<Texture2D> texturesR;
    static std::vector<Texture2D> texturesL;
    Goomba( Vector2 pos, Vector2 dim, Vector2 vel, Color color );
    ~Goomba();
    virtual void update();
    virtual void draw();
    virtual bool checkCollision( Sprite &sprite );
    void updateCollisionProbes();
    static void loadResources();
    static void unloadResources();

};