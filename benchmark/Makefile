export

SHELL := /bin/bash

CC := gcc
CFLAGS := -std=gnu99 -g -pthread -lpthread
RM := rm -f
CPU_NODE := 0
NOW := $(shell date -u +%s)


# to turn on compiling in parallel
ifdef PARALLEL
CCFLAGS += -DPARALLEL
endif

# to turn on mtrace
ifdef MTRACE
CCFLAGS += -DMTRACE
endif

# for NUM_THREADS
ifdef NTHREADS
CCFLAGS += -DNTHREADS=$(NTHREADS)
endif

# for INITIAL
ifdef INITIAL
CCFLAGS += -DINITIAL=$(INITIAL)
endif

# for DIMENSIONS
ifdef DIMENSIONS
CCFLAGS += -DDIMENSIONS=$(DIMENSIONS)
endif

TIME ?= 1# 1 second
WRATIO ?= 0.1
DRATIO ?= 0.5

# for verboseness
ifdef VERBOSE
CCFLAGS += -DVERBOSE
endif

CCFLAGS += -DTIME=$(TIME)LL -DWRATIO=$(WRATIO) -DDRATIO=$(DRATIO) \
	-DHEADER=\"./Quadtree.h\" -DTYPE=Quadtree \
	-DCONSTRUCTOR=Quadtree_init -DDESTRUCTOR=Quadtree_free \
	-DINSERT=Quadtree_add -DQUERY=Quadtree_search -DDELETE=Quadtree_remove

.PHONY: all
all: benchmark.o
	@echo "Object file created: benchmark.o"

%.o: %.c
	$(CC) $(CFLAGS) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-$(RM) benchmark.o
