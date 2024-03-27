#pragma once

#include "GameWorld.h"

void selectionSort( GameWorld* gw );
void insertionSort( GameWorld* gw );
void bubbleSort( GameWorld* gw );
void shellSort( GameWorld* gw );

void mergeSort( GameWorld* gw );
void mergeSortR( GameWorld* gw, int start, int end, int* temp );
void merge( GameWorld* gw, int start, int middle, int end, int* temp );

void quickSort( GameWorld* gw );
void quickSortR( GameWorld* gw, int start, int end );
int partition( GameWorld* gw, int start, int end );

void heapSort( GameWorld* gw );
void sink( GameWorld* gw, int k, int n );

void populateArray( int* array, size_t size );
void shuffeArray( int* array, size_t size );
void copyArray( const int* source, int* destination, size_t size );
void swap( int* array, int p1, int p2 );
