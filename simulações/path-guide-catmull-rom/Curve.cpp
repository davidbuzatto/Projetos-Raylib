#include <iostream>
#include <cmath>
#include <raylib.h>
#include <Curve.h>

void Curve::draw( void ) {
    
    DrawLineV( this->p1, this->p2, GRAY );
    
    DrawSplineSegmentCatmullRom( 
        this->p1, 
        this->c1,
        this->c2,
        this->p2, 
        this->thick, 
        this->color );
    
    this->cp1.draw();
    this->cp2.draw();
    this->pp1.draw();
    this->pp2.draw();

}

void Curve::setP1( float x, float y ) {
    this->p1.x = x;
    this->p1.y = y;
    this->pp1.setPos( x, y );
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
    this->pp2.setPos( x, y );
}

Vector2 &Curve::getP1( void ) {
    return this->p1;
}

Vector2 &Curve::getC1( void ) {
    return this->c1;
}

Vector2 &Curve::getC2( void ) {
    return this->c2;
}

Vector2 &Curve::getP2( void ) {
    return this->p2;
}

ControlPoint *Curve::getPP1( void ) {
    return &(this->pp1);
}

ControlPoint *Curve::getCP1( void ) {
    return &(this->cp1);
}

ControlPoint *Curve::getCP2( void ) {
    return &(this->cp2);
}

ControlPoint *Curve::getPP2( void ) {
    return &(this->pp2);
}

float Curve::getLength( int segments ) {
    
    float q = 1.0 / segments;
    float length = 0;

    for ( int i = 0; i < segments-1; i++ ) {
        //Vector2 p1 = GetSplinePointBezierCubic( 
        Vector2 p1 = GetSplinePointCatmullRom( 
            getP1(), 
            getC1(), 
            getC2(), 
            getP2(), i * q );
        //Vector2 p2 = GetSplinePointBezierCubic( 
        Vector2 p2 = GetSplinePointCatmullRom( 
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
