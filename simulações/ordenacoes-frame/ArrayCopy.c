#include "ArrayCopy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameWorld.h"
#include "SortingAlgorithms.h"

ArrayCopy* createArrayCopy( GameWorld* gw ) {
    
    ArrayCopy* copy = (ArrayCopy*) malloc( sizeof( ArrayCopy ) );
    *copy = (ArrayCopy){0};

    copy->size = gw->size;
    copy->data = (int*) malloc( sizeof(int) * copy->size );
    copyArray( gw->array, copy->data, copy->size );

    return copy;

}

void destroyArrayCopy( ArrayCopy* copy ) {
    free( copy->data );
    free( copy );
}
