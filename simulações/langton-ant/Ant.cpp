/**
 * @file Ant.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Ant class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Ant.h>

#include <iostream>
#include <cmath>
#include <raylib.h>

#include <Direction.h>
#include <TurnType.h>
#include <utils.h>

Ant::Ant( int line, int column ) : 
    line( line ),
    column( column ),
    goingTo( Direction::LEFT ),
    moving( false ),
    drawDecisionCycle( true ) {
}

Ant::~Ant() {
    
}

void Ant::draw() const {

    if ( drawDecisionCycle ) {

        int dCellWidth = 30;
        int dCellSpacing = 20;
        int margin = 10;
        int iMargin = 20;
        int directionSize = dCellWidth * 0.8;

        DrawRectangle( 
            GetScreenWidth() - margin - iMargin * 2 - dCellWidth / 2 - dCellWidth, 
            margin, 
            iMargin * 2 + dCellWidth + dCellWidth / 2, 
            GetScreenHeight() - margin * 2, 
            Fade( WHITE, 0.8 ) );
        DrawRectangleLines( 
            GetScreenWidth() - margin - iMargin * 2 - dCellWidth / 2 - dCellWidth, 
            margin, 
            iMargin * 2 + dCellWidth + dCellWidth / 2, 
            GetScreenHeight() - margin * 2, 
            BLACK );

        Vector2 prev;
        Vector2 first;

        for ( unsigned int i = 0; i < decisionCycle.size(); i++ ) {

            const Decision *d = &decisionCycle[i];
            Vector2 vd( 
                GetScreenWidth() - margin - iMargin - dCellWidth - dCellWidth / 2, 
                margin + iMargin + dCellWidth / 2 + dCellWidth * i + dCellSpacing * i );

            if ( i == 0 ) {
                first = vd;
            }

            int angle = d->getTurnType() == TurnType::TURN_LEFT ? 180 : 0;
            int dx = directionSize * cos( toRadians( angle ) );
            int dy = directionSize * sin( toRadians( angle ) );
            int ax1 = d->getTurnType() == TurnType::TURN_LEFT ? +5 : -5;
            int ay1 = d->getTurnType() == TurnType::TURN_LEFT ? -5 : -5;
            int ax2 = d->getTurnType() == TurnType::TURN_LEFT ? +5 : -5;
            int ay2 = d->getTurnType() == TurnType::TURN_LEFT ? +5 : +5;

            DrawLine( vd.x + dCellWidth / 2, vd.y + dCellWidth / 2, 
                      vd.x + dCellWidth / 2 + dx, 
                      vd.y + dCellWidth / 2 + dy, BLACK );
            DrawLine( vd.x + dCellWidth / 2 + dx, 
                      vd.y + dCellWidth / 2 + dy,
                      vd.x + dCellWidth / 2 + dx + ax1, 
                      vd.y + dCellWidth / 2 + dy + ay1, BLACK );
            DrawLine( vd.x + dCellWidth / 2 + dx, 
                      vd.y + dCellWidth / 2 + dy,
                      vd.x + dCellWidth / 2 + dx + ax2, 
                      vd.y + dCellWidth / 2 + dy + ay2, BLACK );

            DrawRectangle( vd.x, vd.y, dCellWidth, dCellWidth, GetColor( d->getColor() ) );
            DrawRectangleLines( vd.x, vd.y, dCellWidth, dCellWidth, BLACK );

            const char *label = d->getTurnType() == TurnType::TURN_LEFT ? "L" : "R";
            int w = MeasureText( label, 20 );
            DrawText( label, vd.x + dCellWidth / 2 - w / 2, 
                      vd.y + dCellWidth / 2 - 10, 20,
                      getLuminance( d->getColor() ) < 123 ? WHITE : BLACK );

            if ( i != 0 ) {
                DrawLine( prev.x + dCellWidth / 2, prev.y + dCellWidth, vd.x + dCellWidth / 2, vd.y, BLACK );
                DrawLine( vd.x + dCellWidth / 2, vd.y, vd.x + dCellWidth / 2 - 5, vd.y - 5, BLACK );
                DrawLine( vd.x + dCellWidth / 2, vd.y, vd.x + dCellWidth / 2 + 5, vd.y - 5, BLACK );
            }

            prev = vd;

            if ( i == decisionCycle.size() - 1 ) {
                DrawLine( vd.x + dCellWidth / 2, vd.y + dCellWidth, vd.x + dCellWidth / 2, vd.y + dCellWidth + dCellWidth / 2, BLACK );
                DrawLine( vd.x + dCellWidth / 2, vd.y + dCellWidth + dCellWidth / 2, vd.x + dCellWidth + dCellWidth / 2, vd.y + dCellWidth + dCellWidth / 2, BLACK );
                DrawLine( vd.x + dCellWidth + dCellWidth / 2, vd.y + dCellWidth + dCellWidth / 2, first.x + dCellWidth + dCellWidth / 2, first.y - dCellWidth / 2, BLACK );
                DrawLine( first.x + dCellWidth + dCellWidth / 2, first.y - dCellWidth / 2, first.x + dCellWidth / 2, first.y - dCellWidth / 2, BLACK );
                DrawLine( first.x + dCellWidth / 2, first.y - dCellWidth / 2, first.x + dCellWidth / 2, first.y, BLACK );
                DrawLine( first.x + dCellWidth / 2, first.y, first.x + dCellWidth / 2 - 5, first.y - 5, BLACK );
                DrawLine( first.x + dCellWidth / 2, first.y, first.x + dCellWidth / 2 + 5, first.y - 5, BLACK );
            }

        }

    }

    int angle = 0;
    switch ( goingTo ) {
        case Direction::LEFT:
            angle = 180;
            break;
        case Direction::RIGHT:
            angle = 0;
            break;
        case Direction::UP:
            angle = 270;
            break;
        case Direction::DOWN:
            angle = 90;
            break;
    }
    int radius = cellWidth / 1.5;

    Vector2 p( column * cellWidth + cellWidth / 2 - startColumn * cellWidth, 
               line * cellWidth + cellWidth / 2 - startLine * cellWidth );
    Vector2 pe( p.x + radius * cos( toRadians( angle ) ), 
                p.y + radius * sin( toRadians( angle ) ) );
    DrawLineEx( p, pe, radius / 10, MAROON );
    DrawPoly( pe, 3, radius / 4, angle, MAROON );
    DrawCircle( p.x, p.y, radius / 2 < .5 ? 1 : radius / 2, MAROON );

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

void Ant::setDrawDecisionCycle( bool drawDecisionCycle ) {
    this->drawDecisionCycle = drawDecisionCycle;
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

void Ant::setGoingTo( Direction goingTo ) {
    this->goingTo = goingTo;
}

void Ant::addDecision( Decision decision ) {
    decisionCycle.push_back( decision );
}