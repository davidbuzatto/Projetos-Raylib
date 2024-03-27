#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "GameWorld.h"
#include "SortingAlgorithms.h"
#include "raylib.h"

void selectionSort( GameWorld* gw ) {

    int* array = gw->array;
    size_t size = gw->size;

    for ( size_t i = 0; i < size; i++ ) {
        size_t min = i;
        for ( size_t j = i + 1; j < size; j++ ) {
            if ( array[min] > array[j] ) {
                min = j;
            }
        }
        createAndAddNewArrayCopy( gw );
        swap( array, min, i );
    }

}

void insertionSort( GameWorld* gw ) {

    int* array = gw->array;
    size_t size = gw->size;

    for ( size_t i = 1; i < size; i++ ) {
        for ( size_t j = i + 1; j > 0; j-- ) {
            if ( array[j-1] > array[j] ) {
                swap( array, j-1, j );
            } else {
                break;
            }
        }
        createAndAddNewArrayCopy( gw );
    }

}

void bubbleSort( GameWorld* gw ) {

    int* array = gw->array;
    size_t size = gw->size;
    bool swapped;

    for ( size_t i = 0; i < size; i++ ) {
        swapped = false;
        for ( size_t j = size-1; j > i; j-- ) {
            if ( array[j-1] > array[j] ) {
                swap( array, j-1, j );
                swapped = true;
            }
        }
        createAndAddNewArrayCopy( gw );
        if ( !swapped ) {
            break;
        }
    }

}

void shellSort( GameWorld* gw ) {

    int* array = gw->array;
    size_t size = gw->size;
    size_t h = 1;

    while ( h < size / 3 ) {
        h = h * 3 + 1;
    }

    while ( h > 0 ) {
        for ( size_t i = h; i < size; i++ ) {
            for ( size_t j = i; j >= h; j -= h ) {
                if ( array[j-h] > array[j] ) {
                    swap( array, j-h, j );
                } else {
                    break;
                }
            }
            createAndAddNewArrayCopy( gw );
        }
        h /= 3;
    }

}

void mergeSort( GameWorld* gw ) {
    int* temp = (int*) malloc( sizeof(int) * gw->size );
    mergeSortR( gw, 0, gw->size - 1, temp );
    free( temp );
}

void mergeSortR( GameWorld* gw, int start, int end, int* temp ) {
    if ( start < end ) {
        int middle = ( start + end ) / 2;
        mergeSortR( gw, start, middle, temp );
        mergeSortR( gw, middle + 1, end, temp );
        merge( gw, start, middle, end, temp );
    }
}

void merge( GameWorld* gw, int start, int middle, int end, int* temp ) {

    int i = start;
    int j = middle + 1;

    for ( int k = start; k <= end; k++ ) {
        temp[k] = gw->array[k];
    }

    for ( int k = start; k <= end; k++ ) {
        
        if ( i > middle ) {
            gw->array[k] = temp[j++];
        } else if ( j > end ) {
            gw->array[k] = temp[i++];
        } else if ( temp[j] < temp[i] ) {
            gw->array[k] = temp[j++];
        } else {
            gw->array[k] = temp[i++];
        }

        createAndAddNewArrayCopy( gw );

    }

}

void quickSort( GameWorld* gw ) {
    quickSortR( gw, 0, gw->size - 1 );
}

void quickSortR( GameWorld* gw, int start, int end ) {
    if ( start < end ) {
        size_t middle = partition( gw, start, end );
        quickSortR( gw, start, middle - 1 );
        quickSortR( gw, middle + 1, end );
    }
}

int partition( GameWorld* gw, int start, int end ) {

    int i = start;
    int j = end + 1;

    while ( true ) {
        while ( gw->array[++i] < gw->array[start] ) {
            if ( i == end ) {
                break;
            }
        }
        while ( gw->array[--j] > gw->array[start] ) {
            if ( j == start ) {
                break;
            }
        }
        if ( i >= j ) {
            break;
        }
        swap( gw->array, i, j );
        createAndAddNewArrayCopy( gw );
    }

    swap( gw->array, start, j );
    createAndAddNewArrayCopy( gw );

    return j;

}

void heapSort( GameWorld* gw ) {

    int n = gw->size - 1;

    for ( int k = n/2; k >= 0; k-- ) {
        sink( gw, k, n );
    }

    while ( n > 0 ) {
        createAndAddNewArrayCopy( gw );
        swap( gw->array, 0, n-- );
        createAndAddNewArrayCopy( gw );
        sink( gw, 0, n );
    }

}

void sink( GameWorld* gw, int k, int n ) {
    
    while ( 2 * k + 1 <= n ) {

        int j = 2 * k + 1;

        if ( j < n && gw->array[j] < gw->array[j+1] ) {
            j++;
        }

        if ( gw->array[k] >= gw->array[j] ) {
            break;
        }

        createAndAddNewArrayCopy( gw );
        swap( gw->array, k, j );
        k = j;

    }

}


void populateArray( int* array, size_t size ) {
    for ( size_t i = 0; i < size; i++ ) {
        array[i] = (int) (i + 1);
    }
}

void shuffeArray( int* array, size_t size ) {
    for ( size_t i = 0; i < size; i++ ) {
        swap( array, i, GetRandomValue( 0, (int) (size - 1) ) );
    }
}

void copyArray( const int* source, int* destination, size_t size ) {
    for ( size_t i = 0; i < size; i++ ) {
        destination[i] = source[i];
    }
}

void swap( int* array, int p1, int p2 ) {
    int t = array[p1];
    array[p1] = array[p2];
    array[p2] = t;
}