/**
 * @file utils.c
 * @author Prof. Dr. David Buzatto
 * @brief Utilitary functions implementation.
 * 
 * @copyright Copyright (c) 2024
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "raylib.h"
#include "utils.h"

double toRadians( double degrees ) {
    return degrees * PI / 180.0;
}

double toDegrees( double radians ) {
    return radians * 180.0 / PI;
}

void swap( int* array, int p1, int p2 ) {
    int t = array[p1];
    array[p1] = array[p2];
    array[p2] = t;
}