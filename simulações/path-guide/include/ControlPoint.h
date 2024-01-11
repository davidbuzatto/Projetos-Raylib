#pragma once

#include <raylib.h>
#include <Drawable.h>

class ControlPoint : public virtual Drawable {

    Vector2 pos;
    int radius;
    Color color;

public:

    ControlPoint( float x, float y, 
                  float radius, 
                  Color color ) : 
                  pos( { .x = x, .y = y } ),
                  radius( radius ),
                  color( color ) {}

    void setPos( float x, float y );
    const Vector2 &getPos( void );
    bool intercepts( int x, int y );

    virtual void draw( void );
    
};