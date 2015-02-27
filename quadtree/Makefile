CC := gcc
CFLAGS := -std=gnu99 -g
RM := rm -f
TESTFLAG := 

HEADERS := \
	types.h \
	Point.h \
	Quadtree.h

OBJ_SERIAL := \
	serial/SerialSkipQuadtree.o \
	serial/Point.o

OBJS := $(OBJ_SERIAL)

NOW := $(shell date -u +%s)

all: CFLAGS += -O3
all: clean $(OBJS) main.o
	$(CC) $(OBJS) main.o $(CFLAGS) -o quadtree

test: CFLAGS += -O0
test: testcompile
	export MALLOC_TRACE=mtrace/mtrace.$(NOW); ./$@
	$(RM) $@
	mtrace mtrace/mtrace.$(NOW)

test-perf: TESTFLAG := -DVERBOSE
#test-perf: CC := clang
test-perf: test-perf-compile
	numactl --cpunodebind=0 ./test
	#./test
	$(RM) test

test-perf-quick: test-perf-compile test-perf-quick-run
test-perf-quick-run:
	mkdir -p test-perf
	numactl --cpunodebind=0 ./test > test-perf/test-perf-$(NOW).txt
	#./test > test-perf/test-perf-$(NOW).txt
	cat test-perf/test-perf-$(NOW).txt >> test-perf/test-perf.txt
	$(RM) test

#ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
test-perf-compile: CFLAGS += -O0
#test-perf-compile: CFLAGS += -O0 -fsanitize=address
test-perf-compile: $(OBJS) test_perf.o
	$(CC) $(OBJS) test_perf.o $(CFLAGS) -o test

testcompile: TESTFLAG := -DQUADTREE_TEST
testcompile: $(OBJS) test.o
	$(CC) $(OBJS) test.o -DDEBUG -O0 -o test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TESTFLAG)

clean:
	$(RM) $(OBJS) main.o test.o
