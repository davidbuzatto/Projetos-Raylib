/**
 * @file utils.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Utilitary functions implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <raylib.h>
#include <utils.h>

double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}

int getLuminance( unsigned int argb ) {
    return (   77  * ((argb>>16)&255) 
             + 150 * ((argb>>8)&255) 
             + 29  * ((argb)&255))>>8;
}