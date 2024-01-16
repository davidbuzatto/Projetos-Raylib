#include <iostream>
#include <raylib.h>
#include <Curve.h>
#include <ControlPoint.h>

void ControlPoint::draw( void ) {
    DrawCircleV( 
        this->pos,
        this->radius,
        this->color );
}

void ControlPoint::setPos( float x, float y ) {
    this->pos.x = x;
    this->pos.y = y;
}

const Vector2 &ControlPoint::getPos( void ) {
    return this->pos;
}

bool ControlPoint::intercepts( int x, int y ) {
    float c1 = this->pos.x - x;
    float c2 = this->pos.y - y;
    return c1*c1 + c2*c2 < this->radius * this->radius;
}
