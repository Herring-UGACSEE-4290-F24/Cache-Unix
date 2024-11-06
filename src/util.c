#include "inc/util.h"

long parseAddress(long addr, uint8_t start, uint8_t end)
{
  unsigned long mask = ((1UL << (end - start + 1)) - 1) << (start - 1);
  return (addr & mask) >> (start - 1);
}

AddressWidths getAddressWidths(long addr, int totalWidth, int cacheSize, int blockSize, int associativity)
{
  AddressWidths aWidth;
  aWidth.indexWidth = log2(cacheSize / blockSize / associativity);
  aWidth.offsetWidth = log2(blockSize);
  aWidth.tagWidth = totalWidth - (aWidth.offsetWidth + aWidth.indexWidth);

  return aWidth;
}

int getCacheRows(int cacheSize_kb, int blockSize_b)
{
  return (cacheSize_kb * 1024) / blockSize_b;
}

long getBlockAddressDirect(long addr, int blockSizeBytes)
{
  return floorl(addr / blockSizeBytes);
}

int getBlockNumberDirect(long blockAddress, long numOfBlocks)
{
  return blockAddress % numOfBlocks;
}

int allocateCache(int associativity, int totalSize, int blockSize)
{
    return 0;
}

int freeCache()
{
    return 0;
}