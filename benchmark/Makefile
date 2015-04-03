export

SHELL := /bin/bash

CC := gcc
CFLAGS := -std=gnu99 -g -fopenmp
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

COUNT ?= 16384
WRATIO ?= 0.1
DRATIO ?= 0.5

CCFLAGS := -DCOUNT=$(COUNT)LL -DWRATIO=$(WRATIO) -DDRATIO=$(DRATIO) \
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
