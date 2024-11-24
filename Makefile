# Compiler/Debugger options
CC=gcc
DEBUGGER=gdb
CFLAGS=-Wall -I src/inc

# Source and executable names
FILE=src/cache
UTIL=src/util
EXE=$(notdir $(FILE)).out

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the first as argument for gunzip, making sure to set a default of 'art'
  ifeq (,$(word 2, $(MAKECMDGOALS)))
  	TRACE := art
  else
	TRACE := $(word 2, $(MAKECMDGOALS))
  endif
  # use the rest as arguments for cachesim
  CACHE_ARGS := $(wordlist 3,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

build:
	$(CC) $(CFLAGS) $(FILE).c $(UTIL).c -o $(EXE)

run: build
	gunzip -c traces/$(TRACE).trace.gz | ./$(EXE) $(CACHE_ARGS)

debug: build
	$(DEBUGGER) $(EXE)

direct: build
	gunzip -c traces/art.trace.gz | ./$(EXE) -a 1 -s 16 -l 16 -c 2

clean:
	rm -rf $(EXE)
