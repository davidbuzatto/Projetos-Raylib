/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Ant class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Ant.h>

#include <iostream>
#include <fmt/format.h>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <raylib.h>
#include <utils.h>

#include <Direction.h>
#include <TurnType.h>

Ant::Ant() : 
        angle( 300 ),
        moving( false ) {
}

Ant::~Ant() {
    
}

void Ant::draw() const {

    Vector2 p( column * (cellRadius + cellRadius2), 
               line * 2 * cellApothema - ( column % 2 == 0 ? 0 : cellApothema ) );
    /*DrawLine( p.x, p.y, 
              p.x + cellRadius * cos( toRadians( angle - 90 ) ), 
              p.y + cellRadius * sin( toRadians( angle - 90 ) ), 
              BLACK );*/
    DrawCircle( p.x, p.y, 
        cellRadius / 2 < .5 ? 1 : cellRadius / 2, MAROON );

}

void Ant::move( unsigned int *board, int lines, int columns ) {
    
    if ( moving && line >= 0 && line < lines && column >= 0 && column < columns ) {

        int p = line*columns+column;
        unsigned int v = board[p];

        // Langton Ant RL
        for ( unsigned int i = 0; i < decisionCycle.size(); i++ ) {
            
            Decision *d = &decisionCycle[i];
            Decision *n = &decisionCycle[(i+1)%decisionCycle.size()];

            if ( v == d->getColor() ) {
                turn( d->getTurnType() );
                //turn( 0 );
                board[p] = n->getColor();
            }
            
        }

        switch ( angle ) {
            case 0:
                line--;
                break;
            case 60:
                if ( column % 2 == 0 ) {
                    column++;
                } else {
                    line--;
                    column++;
                }
                break;
            case 120:
                if ( column % 2 == 0 ) {
                    line++;
                    column++;
                } else {
                    column++;
                }
                break;
            case 180:
                line++;
                break;
            case 240:
                if ( column % 2 == 0 ) {
                    line++;
                    column--;
                } else {
                    column--;
                }
                break;
            case 300:
                if ( column % 2 == 0 ) {
                    column--;
                } else {
                    line--;
                    column--;
                }
                break;
            default:
                // should never reach this point
                break;
        }

    } else {
        moving = false;
    }

}

void Ant::setLine( int line ) {
    this->line = line;
}

void Ant::setColumn( int column ) {
    this->column = column;
}

void Ant::setCellRadius( float cellRadius ) {
    this->cellRadius = cellRadius;
    this->cellRadius2 = cellRadius/2;
}

void Ant::setCellApothema( float cellApothema ) {
    this->cellApothema = cellApothema;
}

void Ant::setMoving( bool moving ) {
    this->moving = moving;
}

bool Ant::isMoving() {
    return moving;
}

void Ant::turn( int angle ) {
    this->angle = ( this->angle + angle ) % 360;
    //this->angle = angle;
}