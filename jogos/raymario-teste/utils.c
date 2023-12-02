#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "include/raylib.h"

#include "main.h"

CollisionType collide( const Rectangle *r1, const Rectangle *r2 ) {

    CollisionType ct = COLLISION_TYPE_NONE;

    double dx = ( r1->x + r1->width / 2 ) - ( r2->x + r2->width / 2 );
    double dy = ( r1->y + r1->height / 2 ) - ( r2->y + r2->height / 2 );
    double width = ( r1->width + r2->width ) / 2;
    double height = ( r1->height + r2->height ) / 2;
    double crossWidth = width * dy;
    double crossHeight = height * dx;
    
    if( fabs( dx ) <= width && fabs( dy ) <= height ) {
        if( crossWidth > crossHeight ) {
            ct = ( crossWidth > -crossHeight ) ? COLLISION_TYPE_BOTTOM : COLLISION_TYPE_LEFT;
        } else {
            ct = ( crossWidth > -crossHeight ) ? COLLISION_TYPE_RIGHT : COLLISION_TYPE_TOP;
        }
    }

    return ct;

}