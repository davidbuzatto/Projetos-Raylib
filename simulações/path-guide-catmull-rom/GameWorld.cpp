/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <GameWorld.h>

#include <iostream>
#include <fmt/format.h>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <raylib.h>

#include <Curve.h>

/**
 * @brief Construct a new GameWorld object
 */
GameWorld::GameWorld( void ) {

    loadResources();
    std::cout << "creating game world..." << std::endl;

    this->drawingPath = true;
    this->drawingTempCurve = false;
    this->selectedControlPoint = nullptr;
    this->selectedCurve = nullptr;
    this->runningPath = false;
    this->currentCurve = 0;
    this->currentValue = 0;
    this->ball.pos.x = 100;
    this->ball.pos.y = 100;
    this->ball.color = BLUE;
    this->ball.radius = 30;
    this->ball.visible = true;

}

/**
 * @brief Destroy the GameWorld object
 */
GameWorld::~GameWorld( void ) {
    unloadResources();
    std::cout << "destroying game world..." << std::endl;
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void GameWorld::inputAndUpdate( void ) {

    if ( this->drawingPath ) {

        if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            this->xPress = GetMouseX();
            this->yPress = GetMouseY();
        }

        if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) ) {

            float x = GetMouseX();
            float y = GetMouseY();
            std::vector<Curve> curves = this->path.getCurves();

            if ( curves.empty() ) {
                this->tempCurve.setP1( xPress, yPress );
                this->tempCurve.setC1( xPress + ( x - xPress ) / 3, yPress + ( y - yPress ) / 3 );
                this->tempCurve.setC2( x - ( x - xPress ) / 3, y - ( y - yPress ) / 3 );
                this->tempCurve.setP2( x, y );
            } else {
                Curve curve = curves[curves.size()-1];
                this->tempCurve.setP1( curve.getP2().x, curve.getP2().y );
                this->tempCurve.setC1( curve.getP2().x + ( x - curve.getP2().x ) / 3, curve.getP2().y + ( y - curve.getP2().y ) / 3 );
                this->tempCurve.setC2( x - ( x - curve.getP2().x ) / 3, y - ( y - curve.getP2().y ) / 3 );
                this->tempCurve.setP2( x, y );
            }

            this->drawingTempCurve = true;

        }

        if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
            this->path.addCurve( this->tempCurve );
            this->drawingTempCurve = false;
            this->selectedControlPoint = nullptr;
            this->selectedCurve = nullptr;
        }

    } else {

        if ( IsMouseButtonPressed( MOUSE_BUTTON_LEFT ) ) {
            int x = GetMouseX();
            int y = GetMouseY();
            Curve *c = this->path.getInterceptedCurve( x, y );
            if ( c != nullptr ) {
                this->selectedCurve = c;
                if ( c->getPP1()->intercepts( x, y ) ) {
                    this->selectedControlPoint = c->getPP1();
                } else if ( c->getCP1()->intercepts( x, y ) ) {
                    this->selectedControlPoint = c->getCP1();
                } else if ( c->getCP2()->intercepts( x, y ) ) {
                    this->selectedControlPoint = c->getCP2();
                } else if ( c->getPP2()->intercepts( x, y ) ) {
                    this->selectedControlPoint = c->getPP2();
                } else {
                    this->selectedControlPoint = nullptr;
                    this->selectedCurve = nullptr;
                }
            }
        }

        if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) && 
             this->selectedControlPoint != nullptr ) {            
            this->selectedControlPoint->setPos( GetMouseX(), GetMouseY() );
            if ( this->selectedCurve->getPP1() == this->selectedControlPoint ) {
                this->selectedCurve->getP1().x = this->selectedControlPoint->getPos().x;
                this->selectedCurve->getP1().y = this->selectedControlPoint->getPos().y;
            } else if ( this->selectedCurve->getCP1() == this->selectedControlPoint ) {
                this->selectedCurve->getC1().x = this->selectedControlPoint->getPos().x;
                this->selectedCurve->getC1().y = this->selectedControlPoint->getPos().y;
            } else if ( this->selectedCurve->getCP2() == this->selectedControlPoint ) {
                this->selectedCurve->getC2().x = this->selectedControlPoint->getPos().x;
                this->selectedCurve->getC2().y = this->selectedControlPoint->getPos().y;
            } else {
                this->selectedCurve->getP2().x = this->selectedControlPoint->getPos().x;
                this->selectedCurve->getP2().y = this->selectedControlPoint->getPos().y;
            }
            
        }

        if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) ) {
            this->selectedControlPoint = nullptr;
            this->selectedCurve = nullptr;
        }

    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        this->drawingPath = !this->drawingPath;
    }

    if ( IsMouseButtonPressed( MOUSE_BUTTON_RIGHT ) ) {
        std::vector<Curve> curves = this->path.getCurves();
        if ( !curves.empty() ) {
            Curve curve = curves[0];
            this->ball.pos.x = curve.getP1().x;
            this->ball.pos.y = curve.getP1().y;
            this->runningPath = true;
        }
    }

    if ( this->runningPath ) {
        
        std::vector<Curve> curves = this->path.getCurves();

        if ( this->currentCurve < curves.size() ) {
            Curve curve = curves[this->currentCurve];
            float length = curve.getLength( 50 );
            float speed = 5;
            float tick = speed / length;
            std::cout << tick << std::endl;
            if ( this->currentValue <= 1.0 ) {
                //Vector2 p = GetSplinePointBezierCubic( curve.getP1(), curve.getC1(), curve.getC2(), curve.getP2(), this->currentValue );
                Vector2 p = GetSplinePointCatmullRom( curve.getP1(), curve.getC1(), curve.getC2(), curve.getP2(), this->currentValue );
                this->ball.pos.x = p.x;
                this->ball.pos.y = p.y;
                this->currentValue += tick;
                if ( this->currentValue > 1 ) {
                    this->currentValue = 0;
                    this->currentCurve++;
                }
            }
        } else {
            this->currentCurve = 0;
            this->currentValue = 0;
            this->runningPath = false;
        }

    }

}

/**
 * @brief Draws the state of the game.
 */
void GameWorld::draw( void ) {

    BeginDrawing();
    ClearBackground( WHITE );

    this->ball.draw();

    if ( this->drawingTempCurve ) {
        this->tempCurve.draw();
    }

    this->path.draw();
    
    if ( this->drawingPath ) {
        DrawText( "Desenhando caminho", 20, 20, 20, BLACK );
    } else {
        DrawText( "Modificando caminho", 20, 20, 20, BLACK );
    }

    EndDrawing();

}

/**
 * @brief Load game resources like images, textures, sounds, fonts, shaders etc.
 * Should be called inside the constructor.
 */
void GameWorld::loadResources( void ) {
    std::cout << "loading resources..." << std::endl;
}

/**
 * @brief Unload the once loaded game resources.
 * Should be called inside the destructor.
 */
void GameWorld::unloadResources( void ) {
    std::cout << "unloading resources..." << std::endl;
}