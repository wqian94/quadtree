export

SHELL := /bin/bash

CC := gcc
CFLAGS := -std=c99 -g
RM := rm -f
CPU_NODE := 0

NOW := $(shell date -u +%s)

HEADERS := \
	util.h \
	types.h \
	Point.h \
	Quadtree.h

TEST_HEADERS := \
	test.h \
	assertions.h

OBJS_naive := naive/Point.o naive/Quadtree.o
OBJS_serial := serial/Point.o serial/Quadtree.o

.PHONY: all
all:
	@echo "\
\`make all' is not valid. Here are a list of targets to use:\n\
\n\
Make targets:\n\
=============\n\
test-%: run correctness and performance tests on variant %\n\
test-%-correctness: run correctness tests on variant %\n\
test-%-performance: run performance tests on variant %\n\
benchmark-%: run benchmarks on variant %\n\
benchmark-%-O0: run benchmarks on variant % with -O0\n\
benchmark-%-O1: run benchmarks on variant % with -O1\n\
benchmark-%-O2: run benchmarks on variant % with -O2\n\
benchmark-%-O3: run benchmarks on variant % with -O3\n\
main-%: compile main program on variant %\n\
\n\
Variants:\n\
=========\n\
naive: the naive, simple implementation\n\
"

# for no NUMA
ifdef NONUMA
NUMACTL := numactl --cpunodebind=$(CPU_NODE)
endif

# for sanitize
ifdef SANITIZE_ADDRESS
CC := clang
CFLAGS += -fsanitize=address
PRERUN += export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer;
endif

# for OFLAG in benchmarking
OFLAG ?= O3

# for trial counts in benchmarking
TRIALS ?= 1

.PHONY: main-%
main-%:
	$(MAKE) -e run-main

.PHONY: test-%-correctness
test-%-correctness: CFLAGS += -DDEBUG -O0
test-%-correctness: TESTFLAG += -DQUADTREE_TEST
test-%-correctness: PRERUN += export MALLOC_TRACE=mtrace/mtrace.$(NOW);
test-%-correctness:
	$(MAKE) -e run-test OBJS="$(OBJS_$*)"
	mtrace mtrace/mtrace.$(NOW)

.PHONY: test-%-performance
test-%-performance: CFLAGS += -O3
test-%-performance: TESTFLAG += -DVERBOSE
test-%-performance:
	$(MAKE) -e run-test_perf OBJS="$(OBJS_$*)"

.PHONY: test-%
test-%:
	$(MAKE) -e test-$*-correctness
	$(MAKE) -e test-$*-performance

.PHONY: benchmark-%-O0
benchmark-%-O0:
	$(MAKE) -e benchmark-$* OFLAG="O0"

.PHONY: benchmark-%-O1
benchmark-%-O1:
	$(MAKE) -e benchmark-$* OFLAG="O1"

.PHONY: benchmark-%-O2
benchmark-%-O2:
	$(MAKE) -e benchmark-$* OFLAG="O2"

.PHONY: benchmark-%-O3
benchmark-%-O3:
	$(MAKE) -e benchmark-$* OFLAG="O3"

.PHONY: benchmark-%
benchmark-%:
	mkdir -p test-perf/bin test-perf/results
	$(MAKE) -e run-benchmark-test_perf OBJS="$(OBJS_$*)" CFLAGS="$(CFLAGS) -$(OFLAG)"

.PHONY: run-benchmark-%
run-benchmark-%: POSTRUN := >> test-perf/results/test-$(OFLAG)-$(NOW).txt
run-benchmark-%: $(OBJS) compile-% %.o
	mv $* test-perf/bin/test-$(OFLAG)-$(NOW)
	for i in {1..$(TRIALS)};do $(PRERUN) $(NUMACTL) test-perf/bin/test-$(OFLAG)-$(NOW) $(POSTRUN);done

.PHONY: run-%
run-%: $(OBJS) compile-% %.o
	$(PRERUN) $(NUMACTL) ./$* $(POSTRUN)

.PHONY: compile-%
compile-%: %.o
	$(CC) $(CFLAGS) $(OBJS) $*.o -o $*

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TESTFLAG)

.PHONY: clean
clean:
	-$(RM) $(OBJS) main.o test.o *.o
