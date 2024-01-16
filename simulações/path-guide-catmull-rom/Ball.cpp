#include <raylib.h>
#include <Ball.h>

void Ball::draw( void ) {
    if ( this->visible ) {
        DrawCircleV( 
            this->pos,
            this->radius,
            this->color );
    }
}

void Ball::setPos( float x, float y ) {
    this->pos.x = x;
    this->pos.y = y;
}
