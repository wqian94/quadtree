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

all: clean $(OBJS) main.o
	$(CC) $(OBJS) main.o -O3 -o quadtree

test: testcompile
	export MALLOC_TRACE=mtrace/mtrace.$(NOW); ./$@
	$(RM) $@
	mtrace mtrace/mtrace.$(NOW)

test-perf: TESTFLAG := -DQUADTREE_TEST
test-perf: $(OBJS) test_perf.o
	$(CC) $(OBJS) test_perf.o -O3 -o test
	./test
	$(RM) test

test-perf-quick: $(OBJS) test_perf.o
	$(CC) $(OBJS) test_perf.o -O3 -o test
	./test > test-perf/test-perf-$(NOW).txt
	cat test-perf/test-perf-$(NOW).txt >> test-perf/test-perf.txt
	$(RM) test

testcompile: TESTFLAG := -DQUADTREE_TEST
testcompile: $(OBJS) test.o
	$(CC) $(OBJS) test.o -DDEBUG -O0 -o test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TESTFLAG)

clean:
	$(RM) $(OBJS) main.o test.o
