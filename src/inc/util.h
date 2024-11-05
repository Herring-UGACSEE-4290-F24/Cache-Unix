#pragma once

#include "stdlib.h"

// Initial struct for each block of the cache
typedef struct 
{
    int valid;
    int tag;
    int data;
} CacheBlock;

// Given an address and a start and end index, returns an unsigned 32 bit integer that represents the bits in the given
// start and end point
uint32_t parseAddress(int32_t addr, uint8_t start, uint8_t end);

// *** Just in case we need to use the heap ***
// Allocate the cache
int allocateCache(int associativity, int size, int blockSize);

// Free the cache
int freeCache();