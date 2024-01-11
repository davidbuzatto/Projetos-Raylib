#pragma once

#include <raylib.h>
#include <Drawable.h>

class Ball : public virtual Drawable {

public:

    Vector2 pos;
    int radius;
    Color color;
    bool visible;

    Ball() : 
          pos( { .x = 0, .y = 0 } ),
          radius( 20 ),
          color( BLUE ),
          visible( false ) {}

    Ball( float x, float y, 
          float radius, 
          Color color ) : 
          pos( { .x = x, .y = y } ),
          radius( radius ),
          color( color ),
          visible( false ) {}

    void setPos( float x, float y );

    virtual void draw( void );
    
};