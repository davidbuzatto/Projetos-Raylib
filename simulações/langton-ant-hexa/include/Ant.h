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
#include <Decision.h>

class Ant : public virtual Drawable {

    int line;
    int column;
    float cellRadius;
    float cellRadius2;
    float cellApothema;

    int angle;
    bool moving;

    std::vector<Decision> decisionCycle;
    bool drawDecisionCycle;

public:

    /**
     * @brief Construct a new Ant object.
     */
    Ant();

    /**
     * @brief Destroy the Ant object.
     */
    ~Ant();

    virtual void draw() const;

    void move( unsigned int *board, int lines, int columns );

    void setLine( int line );
    void setColumn( int column );

    void setCellRadius( float cellRadius );
    void setCellApothema( float cellApothema );
    void setMoving( bool moving );
    bool isMoving();
    void setDrawDecisionCycle( bool drawDecisionCycle );
    void setAngle( int angle );
    void addDecision( Decision decision );

private:
    void turn( int angle );
    
};