#include "stdio.h"
#include "stdlib.h"

uint32_t parseAddress(uint32_t addr, uint8_t start, uint8_t end)
{
  unsigned long mask = ((1UL << (end - start + 1)) - 1) << (start - 1);
  return (addr & mask) >> (start - 1);
}

int main(void) 
{
    unsigned long addr = 43;

    uint32_t res = parseAddress(addr, 4, 6);

    printf("%lu\n", res);

    return 0;
}