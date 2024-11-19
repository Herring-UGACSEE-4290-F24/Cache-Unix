#include "inc/util.h"
#include <stdint.h>

int log_2(int value) // Should be decently self-explainatory
{
  if (value == 2) {
    return 1;
  } else if (value == 4) {
    return 2;
  } else if (value == 8) {
    return 3;
  } else if (value == 16) {
    return 4;
  } else if (value == 32) {
    return 5;
  } else if (value == 64) {
    return 6;
  } else if (value == 128) {
    return 7;
  } else if (value == 256) {
    return 8;
  } else if (value == 512) {
    return 9;
  } else if (value == 1024) {
    return 10;
  } else if (value == 2048) {
    return 11;
  } else if (value == 4096) {
    return 12;
  } else if (value == 8192) {
    return 13;
  } else {
    return -1;
    // will need to add more if surpasing this amount
  }
}

long parseAddress(long addr, uint8_t start, uint8_t end)
{
  uint8_t length = end - start + 1;                             // uint8_t = "unsigned int "
  unsigned long mask = ((1UL << (length)) - 1) << (start - 1);  // generates a mask of 1's at the bit posistions being fetched
  return (addr & mask) >> (start - 1);                          // returns the fetched bits after shifted to the LSB positions
}

AddressWidths getAddressWidths(int cacheSizeBytes, int blockSizeBytes, int associativity)
{
  AddressWidths addrWidth;
  addrWidth.indexWidth = (int) log_2(cacheSizeBytes / blockSizeBytes / associativity);
  addrWidth.offsetWidth = (int) log_2(blockSizeBytes);
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