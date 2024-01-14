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

#include <Direction.h>
#include <TurnType.h>

Ant::Ant( int line, int column ) : 
        line( line ),
        column( column ),
        goingTo( Direction::LEFT ),
        moving( false ) {
}

Ant::~Ant() {
    
}

void Ant::draw() const {
    DrawCircle( 
        column * cellWidth + cellWidth / 2 - startColumn * cellWidth, 
        line * cellWidth + cellWidth/2 - startLine * cellWidth, 
        cellWidth / 4 < .5 ? 1 : cellWidth / 4, MAROON );
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
                if ( d->getTurnType() == TurnType::TURN_LEFT ) {
                    turnLeft();
                    board[p] = n->getColor();
                } else if ( d->getTurnType() == TurnType::TURN_RIGHT ) {
                    turnRight();
                    board[p] = n->getColor();
                }
            }
            
        }

        
        /*// Base Langton Ant
        if ( v == 0xFFFFFFFF ) {         // white
            board[p] = 0x000000FF;
            turnRight();
        } else if ( v == 0x000000FF ) {  // black
            board[p] = 0xFFFFFFFF;
            turnLeft();
        }*/
        

        switch ( goingTo ) {
            case Direction::LEFT:
                column--;
                break;
            case Direction::RIGHT:
                column++;
                break;
            case Direction::UP:
                line--;
                break;
            case Direction::DOWN:
                line++;
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

void Ant::setCellWidth( int cellWidth ) {
    this->cellWidth = cellWidth;
}

void Ant::setStartLine( int startLine ) {
    this->startLine = startLine;
}

void Ant::setStartColumn( int startColumn ) {
    this->startColumn = startColumn;
}

void Ant::setMoving( bool moving ) {
    this->moving = moving;
}

bool Ant::isMoving() {
    return moving;
}

void Ant::turnRight() {
    switch ( goingTo ) {
        case Direction::LEFT:
            goingTo = Direction::UP;
            break;
        case Direction::RIGHT:
            goingTo = Direction::DOWN;
            break;
        case Direction::UP:
            goingTo = Direction::RIGHT;
            break;
        case Direction::DOWN:
            goingTo = Direction::LEFT;
            break;
    }
}

void Ant::turnLeft() {
    switch ( goingTo ) {
        case Direction::LEFT:
            goingTo = Direction::DOWN;
            break;
        case Direction::RIGHT:
            goingTo = Direction::UP;
            break;
        case Direction::UP:
            goingTo = Direction::LEFT;
            break;
        case Direction::DOWN:
            goingTo = Direction::RIGHT;
            break;
    }
}