/**
 * @file Ant.h
 * @author Prof. Dr. David Buzatto
 * @brief Ant class declaration.
 * 
 * @copyright Copyright (c) 2024
 */
#pragma once

#include <vector>
#include <Drawable.h>
#include <Direction.h>
#include <Decision.h>

class Ant : public virtual Drawable {

    int line;
    int column;
    int cellWidth;

    int startLine;
    int startColumn;

    Direction goingTo;
    bool moving;

    std::vector<Decision> decisionCycle;
    bool drawDecisionCycle;

public:

    /**
     * @brief Construct a new Ant object.
     */
    Ant( int line, int column );

    /**
     * @brief Destroy the Ant object.
     */
    ~Ant();

    virtual void draw() const;

    void move( unsigned int *board, int lines, int columns );

    void setLine( int line );
    void setColumn( int column );

    void setCellWidth( int cellWidth );
    void setStartLine( int startLine );
    void setStartColumn( int startColumn );
    void setMoving( bool moving );
    bool isMoving();
    void setDrawDecisionCycle( bool drawDecisionCycle );

    void addDecision( Decision decision ) {
        decisionCycle.push_back( decision );
    }

private:
    void turnRight();
    void turnLeft();

};