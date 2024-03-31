/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <iostream>
#include <string>

#include "GameWorld.h"
#include "ResourceManager.h"
#include "raylib.h"
//#include "raymath.h"
//#define RAYGUI_IMPLEMENTATION
//#include "raygui.h"
//#undef RAYGUI_IMPLEMENTATION

#include "box2d/box2d.h"

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld() {

    // definition of a ground body
    groundBodyDef.position.Set( 400.0f, 460.0f );

    // creates the static body (attaches to the physics world)
    groundBody = pw.CreateBody( &groundBodyDef );

    // box shape (400 and 50 are halfs of width and height respectively)
    groundBox.SetAsBox( 600.0f, 50.0f );

    // creates a body fixture
    groundBody->CreateFixture( &groundBox, 0.0f );

    // dynamic body
    bodyDef.type = b2_dynamicBody;

    // left
    //bodyDef.position.Set( 129.0f, 20.0f );
    //bodyDef.position.Set( 130.0f, 20.0f );

    // right
    //bodyDef.position.Set( 270.0f, 20.0f );
    //bodyDef.position.Set( 271.0f, 20.0f );
    bodyDef.position.Set( 70.0f, 20.0f );

    body = pw.CreateBody( &bodyDef );
    dynamicBox.SetAsBox( 20.0f, 20.0f );

    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.7f;
    body->CreateFixture( &fixtureDef );

    float width = 10;
    float height = 10;
    float space = 1;
    int max = 19;

    for ( int i = 0; i < max; i++ ) {

        for ( int j = 0; j < 2*i+1; j++ ) {
            
            b2BodyDef newBodyDef;
            newBodyDef.type = b2_dynamicBody;
            newBodyDef.position.Set( 
                ( width * (max-i) *  2 + (max-i) * space ) + 150.0f + ( width * j * 2 ) + space * j, 
                20.0f + ( height * i * 2 ) + ( space * i ) );

            b2Body* newObstacle = pw.CreateBody( &newBodyDef );

            b2PolygonShape newPolygonShape;
            newPolygonShape.SetAsBox( width, height );
            
            b2FixtureDef newFixtureDef;
            newFixtureDef.shape = &newPolygonShape;
            newFixtureDef.density = 1.0f;
            newFixtureDef.friction = 0.3f;

            newObstacle->CreateFixture( &newFixtureDef );

            obstacles.push_back( newObstacle );
            obstacleDefs.push_back( newBodyDef );
            obstacleDynamicBoxes.push_back( newPolygonShape );
            obstacleFixtureDefs.push_back( newFixtureDef );

        }
    }

    /*for ( int i = 0; i < 10; i++ ) {

        b2BodyDef newBodyDef;
        newBodyDef.type = b2_dynamicBody;
        newBodyDef.position.Set( 500.0f + ( larg + 5 ) * i * 2, 20.0f );

        b2Body* newObstacle = pw.CreateBody( &newBodyDef );

        b2PolygonShape newPolygonShape;
        newPolygonShape.SetAsBox( larg, alt );
        
        b2FixtureDef newFixtureDef;
        newFixtureDef.shape = &newPolygonShape;
        newFixtureDef.density = 1.0f;
        newFixtureDef.friction = 0.3f;

        newObstacle->CreateFixture( &newFixtureDef );

        obstacles.push_back( newObstacle );
        obstacleDefs.push_back( newBodyDef );
        obstacleDynamicBoxes.push_back( newPolygonShape );
        obstacleFixtureDefs.push_back( newFixtureDef );

    }*/

}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld() = default;

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate() {

    if ( IsKeyDown( KEY_RIGHT ) ) {
        b2Vec2 f = body->GetWorldVector( b2Vec2( 1000000.0f, 0.0f ) );
        b2Vec2 p = body->GetWorldPoint( b2Vec2( 0, 0 ) );
        body->ApplyForce( f, p, true );
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        b2Vec2 f = body->GetWorldVector( b2Vec2( -1000000.0f, 0.0f ) );
        b2Vec2 p = body->GetWorldPoint( b2Vec2( 0, 0 ) );
        body->ApplyForce( f, p, true );
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        b2Vec2 f = body->GetWorldVector( b2Vec2( 0.0f, -10000000.0f ) );
        b2Vec2 p = body->GetWorldPoint( b2Vec2( 0, 0 ) );
        body->ApplyForce( f, p, true );
    }

    pw.Step( GetFrameTime(), velocityIterations, positionIterations );

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw() {

    BeginDrawing();
    ClearBackground( WHITE );

    b2Vec2 position = body->GetPosition();
    //float angle = body->GetAngle();

    if ( colors.empty() ) {
        float hue = GetRandomValue( 0, 360 );
        for ( size_t i = 0; i < obstacles.size(); i++ ) {
            colors.push_back( ColorFromHSV( hue, 1, static_cast<float>( i+1 ) / obstacles.size() ) );
        }
    }

    DrawRectangle( 
        groundBodyDef.position.x + groundBox.m_vertices->x, 
        groundBodyDef.position.y + groundBox.m_vertices->y, 
        groundBox.m_vertices->x * -2,
        groundBox.m_vertices->y * -2,
        BLUE );

    DrawRectangle( 
        position.x + dynamicBox.m_vertices->x, 
        position.y + dynamicBox.m_vertices->y, 
        dynamicBox.m_vertices->x * -2,
        dynamicBox.m_vertices->y * -2,
        DARKBLUE );

    for ( size_t i = 0; i < obstacles.size(); i++ ) {
        b2Vec2 obstaclePosition = obstacles[i]->GetPosition();
        DrawRectangle( 
            obstaclePosition.x + obstacleDynamicBoxes[i].m_vertices->x, 
            obstaclePosition.y + obstacleDynamicBoxes[i].m_vertices->y, 
            obstacleDynamicBoxes[i].m_vertices->x * -2,
            obstacleDynamicBoxes[i].m_vertices->y * -2,
            colors[i] );
    }

    DrawCircle( 
        groundBodyDef.position.x, 
        groundBodyDef.position.y, 
        5,
        BLACK );

    DrawCircle( 
        position.x, 
        position.y, 
        5,
        BLACK );

    DrawFPS( 20, 20 );

    EndDrawing();

}

/**
 * @brief Load game resources like images, textures, sounds, fonts, shaders etc.
 * Should be called inside the constructor.
 */
void GameWorld::loadResources() {
    ResourceManager::loadResources();
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources() {
    ResourceManager::unloadResources();
}