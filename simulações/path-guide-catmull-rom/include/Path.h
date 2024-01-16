#pragma once

#include <vector>
#include <Drawable.h>
#include <Curve.h>

class Path: public virtual Drawable {

    std::vector<Curve> curves;

public:

    void addCurve( Curve newCurve );
    const std::vector<Curve> &getCurves();
    Curve *getInterceptedCurve( int x, int y );

    virtual void draw( void );

};