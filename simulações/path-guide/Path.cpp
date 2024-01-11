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

        ControlPoint *cp1 = this->curves[i].getCP1();
        ControlPoint *cp2 = this->curves[i].getCP2();

        if ( cp1->intercepts( x, y ) || cp2->intercepts( x, y ) ) {
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