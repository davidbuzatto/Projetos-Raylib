#include <vector>
#include <Curve.h>
#include <Path.h>

void Path::addCurve( Curve curve ) {
    this->curves.push_back( curve );
}

const std::vector<Curve>& Path::getCurves() {
    return this->curves;
}

Curve *Path::getInterceptedCurve( int x, int y ) {

    for ( unsigned int i = 0; i < this->curves.size(); i++ ) {

        ControlPoint *pp1 = this->curves[i].getPP1();
        ControlPoint *cp1 = this->curves[i].getCP1();
        ControlPoint *cp2 = this->curves[i].getCP2();
        ControlPoint *pp2 = this->curves[i].getPP2();

        if ( pp1->intercepts( x, y ) || cp1->intercepts( x, y ) || cp2->intercepts( x, y ) || pp2->intercepts( x, y ) ) {
            return &(this->curves[i]);
        }

    }

    return nullptr;

}

void Path::draw( void ) {
    /*for ( unsigned int i = 0; i < this->curves.size(); i++ ) {
        this->curves[i].draw();
    }*/
    for ( int i = this->curves.size()-1; i >= 0; i-- ) {
        this->curves[i].draw();
    }
}