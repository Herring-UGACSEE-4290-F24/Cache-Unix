#include "inc/util.h"

uint32_t parseAddress(int32_t addr, uint8_t start, uint8_t end)
{
  unsigned long mask = ((1UL << (end - start + 1)) - 1) << (start - 1);
  return (addr & mask) >> (start - 1);
}

int allocateCache(int associativity, int totalSize, int blockSize)
{
    int numberOfLines = (totalSize * 1024) / blockSize;

    return 0;
}

int freeCache()
{
    return 0;
}