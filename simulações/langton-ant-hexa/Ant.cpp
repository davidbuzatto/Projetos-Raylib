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

#include <TurnType.h>
#include <utils.h>

Ant::Ant() : 
    angle( 120 ),
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
        int directionSize = dCellWidth * 0.9;

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
            Vector2 vc( vd.x + dCellWidth / 2, vd.y + dCellWidth / 2 );

            if ( i == 0 ) {
                first = vd;
            }

            int angle = d->getTurnType() - 90;
            int dx = directionSize * cos( toRadians( angle ) );
            int dy = directionSize * sin( toRadians( angle ) );
            int xOffset = 0;
            int ax1 = 0;
            int ay1 = 0;
            int ax2 = 0;
            int ay2 = 0;

            switch ( d->getTurnType() ) {
                case TurnType::TURN_0:
                    xOffset = -dCellWidth / 4;
                    ax1 = -5;
                    ay1 = 5;
                    ax2 = 5;
                    ay2 = 5;
                    break;
                case TurnType::TURN_60:
                    ax1 = -5;
                    ay1 = -2;
                    ax2 = -2;
                    ay2 = 7;
                    break;
                case TurnType::TURN_120:
                    ax1 = -2;
                    ay1 = -7;
                    ax2 = -5;
                    ay2 = 2;
                    break;
                case TurnType::TURN_180:
                    xOffset = -dCellWidth / 4;
                    ax1 = -5;
                    ay1 = -5;
                    ax2 = 5;
                    ay2 = -5;
                    break;
                case TurnType::TURN_240:
                    ax1 = 2;
                    ay1 = -7;
                    ax2 = 5;
                    ay2 = 2;
                    break;
                case TurnType::TURN_300:
                    ax1 = 5;
                    ay1 = -2;
                    ax2 = 2;
                    ay2 = 7;
                    break;
            }

            DrawLine( vd.x + dCellWidth / 2 + xOffset,
                      vd.y + dCellWidth / 2, 
                      vd.x + dCellWidth / 2 + xOffset + dx, 
                      vd.y + dCellWidth / 2 + dy, BLACK );
            DrawLine( vd.x + dCellWidth / 2 + xOffset + dx, 
                      vd.y + dCellWidth / 2 + dy,
                      vd.x + dCellWidth / 2 + xOffset + dx + ax1, 
                      vd.y + dCellWidth / 2 + dy + ay1, BLACK );
            DrawLine( vd.x + dCellWidth / 2 + xOffset + dx, 
                      vd.y + dCellWidth / 2 + dy,
                      vd.x + dCellWidth / 2 + xOffset + dx + ax2, 
                      vd.y + dCellWidth / 2 + dy + ay2, BLACK );

            DrawPoly( vc, 6, dCellWidth / 2 + 2, 0, GetColor( d->getColor() ) );
            DrawPolyLines( vc, 6, dCellWidth / 2 + 2, 0, BLACK );

            std::string label;
            switch ( d->getTurnType() ) {
                case TurnType::TURN_0:
                    label = "N";
                    break;
                case TurnType::TURN_60:
                    label = "R1";
                    break;
                case TurnType::TURN_120:
                    label = "R2";
                    break;
                case TurnType::TURN_180:
                    label = "U";
                    break;
                case TurnType::TURN_240:
                    label = "L2";
                    break;
                case TurnType::TURN_300:
                    label = "L1";
                    break;
            }
            const char *cLabel = label.c_str();
            int w = MeasureText( cLabel, 20 );
            DrawText( cLabel, vd.x + dCellWidth / 2 - w / 2, 
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

    Vector2 p( column * (cellRadius + cellRadius2), 
               line * 2 * cellApothema - ( column % 2 == 0 ? 0 : cellApothema ) );
    Vector2 pe( p.x + cellRadius * cos( toRadians( angle - 90 ) ), 
                p.y + cellRadius * sin( toRadians( angle - 90 ) ) );
    DrawLineEx( p, pe, cellRadius / 10, MAROON );
    DrawPoly( pe, 3, cellRadius / 4, angle - 90, MAROON );
    DrawCircle( p.x, p.y, cellRadius / 2 < .5 ? 1 : cellRadius / 2, MAROON );

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

void Ant::setDrawDecisionCycle( bool drawDecisionCycle ) {
    this->drawDecisionCycle = drawDecisionCycle;
}

void Ant::turn( int angle ) {
    this->angle = ( this->angle + angle ) % 360;
}

void Ant::setAngle( int angle ) {
    this->angle = angle;
}

void Ant::addDecision( Decision decision ) {
    decisionCycle.push_back( decision );
}