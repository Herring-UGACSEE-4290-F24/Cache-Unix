#include "inc/util.h"

long parseAddress(long addr, uint8_t start, uint8_t end)
{
  unsigned long mask = ((1UL << (end - start + 1)) - 1) << (start - 1);
  return (addr & mask) >> (start - 1);
}

AddressWidths getAddressWidths(long addr, int cacheSize, int blockSize, int associativity)
{
  AddressWidths addrWidth;
  addrWidth.indexWidth = log2(cacheSize / blockSize / associativity);
  addrWidth.offsetWidth = log2(blockSize);
  addrWidth.tagWidth = ADDR_BIT_LENGTH - (addrWidth.offsetWidth + addrWidth.indexWidth);

  return addrWidth;
}

int getCacheBlocks(int cacheSize_kb, int blockSize_b) // Suggestion: Change this to getNumBlocks
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