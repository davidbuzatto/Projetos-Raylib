#include <iostream>
#include <cmath>
#include <raylib.h>
#include <Curve.h>

void Curve::draw( void ) {
    
    DrawSplineSegmentBezierCubic( 
        this->p1, 
        this->c1,
        this->c2,
        this->p2, 
        this->thick, 
        this->color );
    
    this->cp1.draw();
    this->cp2.draw();

}

void Curve::setP1( float x, float y ) {
    this->p1.x = x;
    this->p1.y = y;
}

void Curve::setC1( float x, float y ) {
    this->c1.x = x;
    this->c1.y = y;
    this->cp1.setPos( x, y );
}

void Curve::setC2( float x, float y ) {
    this->c2.x = x;
    this->c2.y = y;
    this->cp2.setPos( x, y );
}

void Curve::setP2( float x, float y ) {
    this->p2.x = x;
    this->p2.y = y;
}

const Vector2 &Curve::getP1( void ) {
    return this->p1;
}

Vector2 &Curve::getC1( void ) {
    return this->c1;
}

Vector2 &Curve::getC2( void ) {
    return this->c2;
}

const Vector2 &Curve::getP2( void ) {
    return this->p2;
}

ControlPoint *Curve::getCP1( void ) {
    return &(this->cp1);
}

ControlPoint *Curve::getCP2( void ) {
    return &(this->cp2);
}

float Curve::getLength( int segments ) {
    
    float q = 1.0 / segments;
    float length = 0;

    for ( int i = 0; i < segments-1; i++ ) {
        Vector2 p1 = GetSplinePointBezierCubic( 
            getP1(), 
            getC1(), 
            getC2(), 
            getP2(), i * q );
        Vector2 p2 = GetSplinePointBezierCubic( 
            getP1(), 
            getC1(), 
            getC2(), 
            getP2(), (i+1) * q );
        float c1 = p1.x - p2.x;
        float c2 = p1.y - p2.y;
        length += sqrt( c1*c1 + c2*c2 );
    }

    return length;

}
