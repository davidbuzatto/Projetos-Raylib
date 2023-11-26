#ifndef BASE_H
#define BASE_H

//#pragma once

#include "include/raylib.h"

typedef enum {
    COLLISION_NONE,
    COLLISION_LEFT,
    COLLISION_RIGHT,
    COLLISION_TOP,
    COLLISION_BOTTOM
} CollisionResult;

typedef struct {
    double x;
    double y;
    double width;
    double height;
    double vx;
    double vy;
    double angle;
    Color baseColor;
} SpriteData;

#endif // BASE_H