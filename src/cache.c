#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <math.h>
#include "inc/util.h"

/*
 *
 * Simulator Knob Definitions
 * 
 */

int associativity = 2;    // Associativity of cache
int blocksize_bytes = 32; // Cache Block size in bytes
int cachesize_kb = 64;    // Cache size in KB
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
int memory_accesses = 0;            // would this be for both load and store or just load?
double overall_miss_rate = 0;       // percentatge?
double overall_read_miss_rate = 0;  
int memory_cpi = 0;                 // what is this?
int total_cpi = 0;
int avg_mem_access_time = 0;        // unit defined as number of cycles
int dirty_evictions = 0;
int load_misses = 0;
int store_misses = 0;
int load_hits = 0;
int store__hits = 0;

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

  // Set default cache values
  for (size_t i = 0; i < cacheRows; i++)
  {
    for (size_t j = 0; j < associativity; j++)
    {
      directCache[i][j].valid = 0; // 0 is for invalid, 1 is for valid
      directCache[i][j].tag = 0;
      directCache[i][j].LRU = 0;
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
    if (i < 10)
    {
      printf("\t%c %d %lx %d\n", marker, loadstore, address, icount);

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
  // Here is where you want to print out stats
  printf("Lines found = %i \n", i);
  printf("Simulation results:\n");
  //  Use your simulator to output the following statistics.  The
  //  print statements are provided, just replace the question marks with
  //  your calcuations.

  
  //printf("\texecution time %ld cycles\n", ?);
  printf("\tinstructions:    %d\n", total_instructions);
  printf("\tmemory accesses: %d\n", memory_accesses);
  //printf("\toverall miss rate %.2f\n", ? );
  //printf("\tread miss rate %.2f\n", ? );
  //printf("\tmemory CPI %.2f\n", ?);
  //printf("\ttotal CPI %.2f\n", ?);
  //printf("\taverage memory access time %.2f cycles\n",  ?);
  //printf("dirty evictions %d\n", ?);
  //printf("load_misses %d\n", ?);
  //printf("store_misses %d\n", ?);
  //printf("load_hits %d\n", ?);
  //printf("store_hits %d\n", ?);
}
