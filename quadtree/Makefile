export

CC := gcc
CFLAGS := -std=c99 -g
RM := rm -f
TESTFLAG := 
PRERUN := 
POSTRUN := 
NONUMA := 
OBJS := 

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
main-%: compile main program on variant %\n\
\n\
Variants:\n\
=========\n\
naive: the naive, simple implementation\n\
"

# for no NUMA
ifdef NONUM
NONUMA := numactl --cpunodebind=0
endif

# for sanitize
ifdef SANITIZE_ADDRESS
CC := clang
CFLAGS += -fsanitize=address
PRERUN += export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer;
endif

.PHONY: main-%
main-%: CFLAGS += -O3
main-%:
	$(MAKE) run-main

.PHONY: test-%-correctness
test-%-correctness: CFLAGS += -DDEBUG -O0
test-%-correctness: TESTFLAG += -DQUADTREE_TEST
test-%-correctness: PRERUN += export MALLOC_TRACE=mtrace/mtrace.$(NOW);
test-%-correctness: OBJS := OBJS_$*
test-%-correctness:
	$(MAKE) run-test
	mtrace mtrace/mtrace.$(NOW)

.PHONY: test-%-performance
test-%-performance: CFLAGS += -O3
test-%-performance: OBJS := OBJS_$*
test-%-performance: TESTFLAG += -DVERBOSE
test-%-performance:
	$(MAKE) run-test

.PHONY: test-%
test-%:
	$(MAKE) test-$*-correctness
	$(MAKE) test-$*-performance

.PHONY: benchmark-%
benchmark-%: PRERUN += mkdir -p benchmark;
benchmark-%: POSTRUN += > test-perf/test-perf-$(NOW).txt
benchmark-%: OBJS := OBJS_$*
benchmark-%:
	mkdir -p test-perf
	$(MAKE) run-test_perf
	cat test-perf/test-perf-$(NOW).txt >> test-perf/test-perf.txt

.PHONY: run-%
run-%: $(OBJS) compile-% %.o
	$(PRERUN); $(NONUMA) ./% $(POSTRUN)

.PHONY: compile-%
compile-%: %.o
	$(CC) $(CFLAGS) $(OBJS) %.o -o %

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TESTFLAG)

.PHONY: clean
clean:
	-$(RM) $(OBJS) main.o test.o
