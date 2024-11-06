#pragma once

#include "stdlib.h"
#include "math.h"

// Initial struct for each block of the cache
typedef struct 
{
    int valid;
    int tag;
    int data;
} CacheBlock;

typedef struct
{
    int tagWidth;
    int indexWidth;
    int offsetWidth;
} AddressWidths;

// Given an address and a start and end index, returns a long that represents the bits in the given
// start and end point
long parseAddress(long addr, uint8_t start, uint8_t end);


AddressWidths getAddressWidths(long addr, int totalWidth, int cacheSize, int blockSize, int associativity);

// Calculates how many rows needed in the 2D array
int getCacheRows(int cacheSize_kb, int blockSize_b);

// For direct mapped associativity, used to get location for storage
long getBlockAddressDirect(long addr, int blockSizeBytes);
int getBlockNumberDirect(long blockAddress, long numOfBlocks);

// *** Just in case we need to use the heap ***
// Allocate the cache
int allocateCache(int associativity, int size, int blockSize);

// Free the cache
int freeCache();