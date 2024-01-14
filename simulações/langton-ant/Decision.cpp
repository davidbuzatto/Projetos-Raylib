/**
 * @file GameWorld.cpp
 * @author Prof. Dr. David Buzatto
 * @brief Decision class implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <Decision.h>

#include <iostream>
#include <fmt/format.h>
#include <cmath>
#include <string>
#include <cstring>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <raylib.h>

#include <TurnType.h>

Decision::Decision( unsigned int color, TurnType turnType ) : 
        color( color ),
        turnType( turnType ) {
}

Decision::~Decision() {
    
}

TurnType Decision::getTurnType() const {
    return turnType;
}

unsigned int Decision::getColor() const {
    return color;
}