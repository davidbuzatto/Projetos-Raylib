/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class declaration. This class should contain all
 * game components and its state.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include "Drawable.h"
#include "box2d/box2d.h"
#include "raylib.h"
#include <vector>

class GameWorld : public virtual Drawable {

    b2Vec2 gravity{ 0.0f, 100.0f };
    b2World pw{ gravity }; // pw -> physics world

    int32 velocityIterations{ 8 };
    int32 positionIterations{ 3 };

    b2BodyDef groundBodyDef;
    b2Body* groundBody;
    b2PolygonShape groundBox;

    b2BodyDef bodyDef;
    b2Body* body;
    b2PolygonShape dynamicBox;
    b2FixtureDef fixtureDef;


    b2BodyDef obstacleDef;
    b2Body* obstacle;
    b2PolygonShape obstacleDynamicBox;
    b2FixtureDef obstacleFixtureDef;

    std::vector<b2BodyDef> obstacleDefs;
    std::vector<b2Body*> obstacles;
    std::vector<b2PolygonShape> obstacleDynamicBoxes;
    std::vector<b2FixtureDef> obstacleFixtureDefs;
    std::vector<Color> colors;
    
public:

    /**
     * @brief Construct a new GameWorld object.
     */
    GameWorld();

    /**
     * @brief Destroy the GameWorld object.
     */
    virtual ~GameWorld();

    /**
     * @brief Reads user input and updates the state of the game.
     */
    void inputAndUpdate();

    /**
     * @brief Draws the state of the game.
     */
    void draw() override;

    /**
     * @brief Load game resources like images, textures, sounds, fonts, shaders,
     * etc.
     * Should be called inside the constructor.
     */
    static void loadResources();

    /**
     * @brief Unload the once loaded game resources.
     * Should be called inside the destructor.
     */
    static void unloadResources();
    
};