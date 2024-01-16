#pragma once

#include <raylib.h>
#include <Drawable.h>
#include <Curve.h>
#include <ControlPoint.h>

class Curve : public virtual Drawable {

    Vector2 p1;
    Vector2 c1;
    Vector2 c2;
    Vector2 p2;
    ControlPoint pp1;
    ControlPoint cp1;
    ControlPoint cp2;
    ControlPoint pp2;
    float thick;
    Color color;

public:

    Curve() : p1( { .x = 0, .y = 0 } ),
              c1( { .x = 0, .y = 0 } ),
              c2( { .x = 0, .y = 0 } ),
              p2( { .x = 0, .y = 0 } ),
              pp1( 0, 0, 5, BLACK ),
              cp1( 0, 0, 5, ORANGE ),
              cp2( 0, 0, 5, LIME ),
              pp2( 0, 0, 5, BLACK ),
              thick( 2 ),
              color( BLACK ) {
    }
    
    Curve( float p1x, float p1y,
           float p2x, float p2y,
           float thick,
           Color color ) : 
           p1( { .x = p1x, .y = p1y } ),
           c1( { .x = p1x, .y = p1y } ),
           c2( { .x = p2x, .y = p2y } ),
           p2( { .x = p2x, .y = p2y } ),
           pp1( p1x, p1y, 5, BLACK ),
           cp1( p1x, p1y, 5, ORANGE ),
           cp2( p2x, p2y, 5, LIME ),
           pp2( p2x, p2y, 5, LIME ),
           thick( thick ),
           color( color ) {
    }

    void setP1( float x, float y );
    void setC1( float x, float y );
    void setC2( float x, float y );
    void setP2( float x, float y );

    Vector2 &getP1( void );
    Vector2 &getC1( void );
    Vector2 &getC2( void );
    Vector2 &getP2( void );
    ControlPoint *getPP1( void );
    ControlPoint *getCP1( void );
    ControlPoint *getCP2( void );
    ControlPoint *getPP2( void );

    float getLength( int segments );

    void update( void );

    virtual void draw( void );
    
};