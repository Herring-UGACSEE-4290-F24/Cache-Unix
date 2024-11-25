#pragma once

#include "stdlib.h"
#include "math.h"
#include <stdint.h>

#define ADDR_BIT_LENGTH 32 // Length of each trace address

// Initial struct for each block of the cache
typedef struct 
{
    int valid;
    int dirty;
    int tag;
    int LRU;
} CacheBlock;

// Struct that represents the bit widths for the given address
typedef struct
{
    int tagWidth;
    int indexWidth;
    int offsetWidth;
} AddressWidths;

// Lookup function for log with base 2 since Jake's computer
// won't run the built in one
int log_2(int value);

// Given an address and a start and end index, returns a long that represents the bits in the given
// start and end point
long parseAddress(long addr, uint8_t start, uint8_t end);

// Calculates the address widths given the initial parameters
AddressWidths getAddressWidths(int cacheSizeBytes, int blockSizeBytes, int associativity);

// Calculates how many blocks in the 2D array
int getTotalCacheBlocks(int cacheSize_kb, int blockSize_b);

// For direct mapped associativity, used to get location for storage
long getBlockAddressDirect(long addr, int blockSizeBytes);
int getBlockNumberDirect(long blockAddress, long numOfBlocks);