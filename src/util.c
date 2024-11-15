#include "inc/util.h"

long parseAddress(long addr, uint8_t start, uint8_t end)
{
  uint8_t length = end - start + 1;                             // uint8_t = "unsigned int "
  unsigned long mask = ((1UL << (length)) - 1) << (start - 1);  // generates a mask of 1's at the bit posistions being fetched
  return (addr & mask) >> (start - 1);                          // returns the fetched bits after shifted to the LSB positions
}

AddressWidths getAddressWidths(int cacheSizeBytes, int blockSizeBytes, int associativity)
{
  AddressWidths addrWidth;
  addrWidth.indexWidth = (int)log2(cacheSizeBytes / blockSizeBytes / associativity);
  addrWidth.offsetWidth = (int)log2(blockSizeBytes);
  addrWidth.tagWidth = ADDR_BIT_LENGTH - (addrWidth.offsetWidth + addrWidth.indexWidth);

  return addrWidth;
}

int getTotalCacheBlocks(int cacheSize_kb, int blockSize_b) // Suggestion: Change this to getNumBlocks
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