#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "inc/util.h"

/*
 *
 * Simulator Knob Definitions
 * 
 */

int associativity = 1;    // Associativity of cache
int blocksize_bytes = 16; // Cache Block size in bytes
int cachesize_kb = 16;    // Cache size in KB
int miss_penalty = 30;
float clockRate = 2;      // Clock speed in GHz

/*
 *
 * Additional Intitial Params (for devising performance metrics of cache)
 * 
 */
int total_instructions = 0;
int total_cycles = 0;
int execution_time = 0;             // i.e. total CPU time
int memory_accesses = 0;            
double overall_miss_rate = 0;       // percentatge?
double overall_read_miss_rate = 0;  
int memory_cpi = 0;                 // what is this?
int total_cpi = 0;
int avg_mem_access_time = 0;        // unit defined as number of cycles
int dirty_evictions = 0;
int load_misses = 0;
int store_misses = 0;
int load_hits = 0;
int store_hits = 0;

/*
 * Should we define other penalties here?
 * e.g. Dirty line eviction = 2 clk penality
 *      load w/ dirty line eviction = miss_delay + 2 clks
 *      new load evicting clean line = miss_delay
 */



void print_usage()
{
  printf("Usage: gunzip2 -c <tracefile> | ./cache -a <assoc> -l <blksz> -s <size> -mp <mispen>\n");
  printf("  <tracefile>: The memory trace file\n");
  printf("  -a <assoc>: The associativity of the cache\n");
  printf("  -l <blksz>: The blocksize (in bytes) of the cache\n");
  printf("  -s <size>: The size (in KB) of the cache\n");
  printf("  -mp <mispen>: The miss penalty (in cycles) of a miss\n");
  exit(0);
}

int main(int argc, char *argv[])
{

  long address;
  int loadstore, icount;
  char marker;

  int i = 0;
  int j = 1;

  // Process the command line arguments
  // Process the command line arguments
  while (j < argc)
  {
    if (strcmp("-a", argv[j]) == 0)
    {
      j++;
      if (j >= argc)
        print_usage();
      associativity = atoi(argv[j]);
      j++;
    }
    else if (strcmp("-l", argv[j]) == 0)
    {
      j++;
      if (j >= argc)
        print_usage();
      blocksize_bytes = atoi(argv[j]);
      j++;
    }
    else if (strcmp("-s", argv[j]) == 0)
    {
      j++;
      if (j >= argc)
        print_usage();
      cachesize_kb = atoi(argv[j]);
      j++;
    }
    else if (strcmp("-mp", argv[j]) == 0)
    {
      j++;
      if (j >= argc)
        print_usage();
      miss_penalty = atoi(argv[j]);
      j++;
    }
    else
    {
      print_usage();
    }
  }

  
  int cacheBlocks = getTotalCacheBlocks(cachesize_kb, blocksize_bytes);
  // ^ this value is the amount of blocks in the cache
  // Rows would be determined with Blocks/Associativity
  
  // Direct Mapped Cache
  CacheBlock directCache[cacheBlocks][1];
  
  // Initialize cache
  // CacheBlock cache[cacheBlocks / associativity][associativity];

  int cacheRows = cacheBlocks / associativity;

  printf("Total Cache Rows: %d\n", cacheRows);

  // Set default cache values
  for (size_t i = 0; i < cacheRows; i++)
  {
    for (size_t j = 0; j < associativity; j++)
    {
      directCache[i][j].valid = 0; // 0 is for invalid, 1 is for valid
      directCache[i][j].tag = 0;
      directCache[i][j].LRU = 0;
      directCache[i][j].dirty = 0;
    }
  } 

  AddressWidths addrWidth = getAddressWidths(cachesize_kb * 1024, blocksize_bytes, associativity);

  printf("Index Width:  %d\n", addrWidth.indexWidth);
  printf("Offset Width: %d\n", addrWidth.offsetWidth);
  printf("Tag Width:    %d\n", addrWidth.tagWidth);

  // print out cache configuration
  printf("Cache parameters:\n");
  printf("Cache Size (KB)\t\t\t%d\n", cachesize_kb);
  printf("Cache Associativity\t\t%d\n", associativity);
  printf("Cache Block Size (bytes)\t%d\n", blocksize_bytes);
  printf("Miss penalty (cyc)\t\t%d\n", miss_penalty);
  printf("\n");

  while (scanf("%c %d %lx %d\n", &marker, &loadstore, &address, &icount) != EOF)
  {
    // Code to print out just the first 10 addresses.  You'll want to delete
    // this part once you get things going.
    if (1) // swap "i < 10" to "1" for full test
    {
      printf("\t%c %d %lx %d\n", marker, loadstore, address, icount);

      // If errors - tag: 15, 32    index: 5, 14
      long tag = parseAddress(address, 32 - addrWidth.tagWidth + 1, 32);
      long index = parseAddress(address, addrWidth.offsetWidth + 1, addrWidth.offsetWidth + addrWidth.indexWidth);

      printf("\t\t-Tag:   %ld\n", tag);
      printf("\t\t-Index: %ld\n", index);

      if (directCache[index][0].tag != tag || directCache[index][0].valid == 0) 
      { // when tag doesn't match or index is invalid
        printf("\t\t-MISS\n");
        directCache[index][0].tag = tag;
        directCache[index][0].valid = 1;

        if (directCache[index][0].dirty) {
          // NEED TO WRITE-BACK
          // "Dirty eviction"
          dirty_evictions++;
          directCache[index][0].dirty = 0;
        }

        if (loadstore) 
        {
          store_misses++;
          directCache[index][0].dirty = 1;
        } 
        else 
        {
          load_misses++;
        }
      } 
      else if (directCache[index][0].tag == tag)
      { // when tag matches
        printf("\t\t-HIT\n");

        if (loadstore) 
        {
          store_hits++;
          directCache[index][0].dirty = 1;
        }
        else
        {
          load_hits++;
        }
      }

      i++;
    } 
    else
    {
      //return 0;
      break;
    }

    // here is where you will want to process your memory accesses

    total_instructions += icount;
    memory_accesses++;
  }
  // Processing Stats:
  overall_miss_rate = (load_misses + store_misses) / (double) memory_accesses;
  overall_read_miss_rate = load_misses / (double) (load_misses + load_hits);
  total_cycles += total_instructions;

  // Here is where you want to print out stats
  printf("Lines found = %i \n", i);
  printf("Simulation results:\n");
  //  Use your simulator to output the following statistics.  The
  //  print statements are provided, just replace the question marks with
  //  your calcuations.

  //printf("\texecution time %ld cycles\n", ?);
  printf("\tinstructions:      %d\n", total_instructions);
  printf("\tmemory accesses:   %d\n", memory_accesses);
  printf("\toverall miss rate: %.2f\n", overall_miss_rate);
  printf("\tread miss rate:    %.2f\n", overall_read_miss_rate);
  //printf("\tmemory CPI     %.2f\n", ?);
  //printf("\ttotal CPI      %.2f\n", ?);
  //printf("\taverage memory access time %.2f cycles\n",  ?);
  printf("\tdirty evictions:   %d\n", dirty_evictions);
  printf("\tload_misses:       %d\n", load_misses);
  printf("\tstore_misses:      %d\n", store_misses);
  printf("\tload_hits:         %d\n", load_hits);
  printf("\tstore_hits:        %d\n", store_hits);
}
