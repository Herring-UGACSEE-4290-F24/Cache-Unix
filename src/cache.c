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

int associativity = 1;        // Associativity of cache
int blocksize_bytes = 16;     // Cache Block size in bytes
int cachesize_kb = 16;        // Cache size in KB
int miss_penalty = 30;        // This won't be getting updated (need to change clockRate to affect this)
float clockRate = 2;      // Clock speed in GHz

/*
 *
 * Additional Intitial Params (for devising performance metrics of cache)
 * 
 */
int total_instructions = 0;         // Number of instructions executed (mem + non-mem)
long int total_cycles = 0;          // How many total cycles are taken (includes non-mem instructions)
long int mem_cycles = 0;            // How many cycles memory instructions take
int execution_time = 0;             // i.e. total CPU time
int memory_accesses = 0;            // number of times memory is accessed
double overall_miss_rate = 0;       // Percent of miss/toatal access
double overall_read_miss_rate = 0;  // Percent of read miss / total access
double memory_cpi = 0;              // mem_cycles/total instructions
double total_cpi = 0;               // total_cycles/total_instructions
double avg_mem_access_time = 0;     // unit defined as number of cycles
int dirty_evictions = 0;            // times that a dirty line is evicted
int load_misses = 0;                // misses on load instructions
int store_misses = 0;               // misses on store instructions
int load_hits = 0;                  // hits on load instructions
int store_hits = 0;                 // hits on store instructions

void print_usage()
{
  printf("Usage: gunzip2 -c <tracefile> | ./cache -a <assoc> -l <blksz> -s <size> -c <clkspd>\n");
  printf("  <tracefile>: The memory trace file\n");
  printf("  -a <assoc>: The associativity of the cache\n");
  printf("  -l <blksz>: The blocksize (in bytes) of the cache\n");
  printf("  -s <size>: The size (in KB) of the cache\n");
  printf("  -c <clkspd>: The speed of the clock (in GHz)\n");
  exit(0);
}

int main(int argc, char *argv[])
{

  long address;     // The address being passed in by the trace       
  int loadstore;    // Whether the mem instruction is a load (0) or store (1)
  int icount;       // Instruction count - number of instructions executed
  char marker;      // Char that denotes the start of a trace line

  int i = 0;        // Iterator for the trace lines
  int j = 1;        // Iterator to scan the arguments passed in on the terminal

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
    else if (strcmp("-c", argv[j]) == 0)
    {
      j++;
      if (j >= argc)
        print_usage();
      clockRate = atoi(argv[j]);
      j++;
    }
    else
    {
      print_usage();
    }
  }

  miss_penalty = clockRate * 15;    // Sets the base miss penalty based on clock speed
  switch (blocksize_bytes)          // Adds the extra penalty for larger block sizes
  { 
  case 32:
    miss_penalty += 2;
    break;
  
  case 64:
    miss_penalty += 6;
    break;

  case 128:
    miss_penalty += 14;
    break;

  default:
    miss_penalty += 0;
    break;
  }
  
  int cacheBlocks = getTotalCacheBlocks(cachesize_kb, blocksize_bytes);
  // ^ this value is the amount of blocks in the cache
  // Rows would be determined with Blocks/Associativity
  
  // Initialize cache
  CacheBlock cache[cacheBlocks / associativity][associativity];

  int cacheRows = cacheBlocks / associativity;

  printf("Total Cache Rows: %d\n", cacheRows);

  // Set default cache values
  for (size_t i = 0; i < cacheRows; i++)
  {
    for (size_t j = 0; j < associativity; j++)
    {
      cache[i][j].valid = 0; // 0 is for invalid, 1 is for valid
      cache[i][j].tag = 0;
      cache[i][j].LRU = 0;
      cache[i][j].dirty = 0;
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

      if (cache[index][0].tag != tag || cache[index][0].valid == 0) 
      { // when tag doesn't match or index is invalid
        printf("\t\t-MISS\n");
        cache[index][0].tag = tag;
        cache[index][0].valid = 1;

        mem_cycles += miss_penalty;

        if (cache[index][0].dirty) {
          // NEED TO WRITE-BACK
          mem_cycles += 2;
          // "Dirty eviction"
          dirty_evictions++;
          cache[index][0].dirty = 0;
        }

        if (loadstore) 
        {
          store_misses++;
          cache[index][0].dirty = 1;
        } 
        else 
        {
          load_misses++;
        }
      } 
      else if (cache[index][0].tag == tag)
      { // when tag matches
        printf("\t\t-HIT\n");

        if (loadstore) 
        {
          store_hits++;
          cache[index][0].dirty = 1;
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
  total_cycles = total_instructions + mem_cycles;
  avg_mem_access_time = mem_cycles / (double) memory_accesses;
  memory_cpi = mem_cycles / (double) total_instructions;
  total_cpi = total_cycles / (double) total_instructions;

  // Here is where you want to print out stats
  printf("Lines found = %i \n", i);
  printf("Simulation results:\n");
  //  Use your simulator to output the following statistics.  The
  //  print statements are provided, just replace the question marks with
  //  your calcuations.

  printf("\texecution time:    %ld cycles\n", total_cycles);
  printf("\tinstructions:      %d\n", total_instructions);
  printf("\tmemory accesses:   %d\n", memory_accesses);
  printf("\toverall miss rate: %.2f\n", overall_miss_rate);
  printf("\tread miss rate:    %.2f\n", overall_read_miss_rate);
  printf("\tmemory CPI         %.2f\n", memory_cpi);
  printf("\ttotal CPI          %.2f\n", total_cpi);
  printf("\taverage memory access time: %.2f cycles\n",  avg_mem_access_time);
  printf("\tdirty evictions:   %d\n", dirty_evictions);
  printf("\tload_misses:       %d\n", load_misses);
  printf("\tstore_misses:      %d\n", store_misses);
  printf("\tload_hits:         %d\n", load_hits);
  printf("\tstore_hits:        %d\n", store_hits);
}
