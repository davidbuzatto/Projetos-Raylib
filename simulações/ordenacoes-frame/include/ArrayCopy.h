#pragma once

#include <stdio.h>
#include <stdlib.h>

struct GameWorld;

typedef struct ArrayCopy {
    int* data;
    size_t size;
    struct ArrayCopy* previous;
} ArrayCopy;

ArrayCopy* createArrayCopy( struct GameWorld* gw );
void destroyArrayCopy( ArrayCopy* copy );