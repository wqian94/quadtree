CC := gcc
CFLAGS := -std=gnu99 -g
RM := rm -f
TESTFLAG := 

HEADERS := \
	types.h \
	Point.h \
	KDtree.h

OBJ_SERIAL := \
	serial/SerialSkipKDtree.o \
	serial/Point.o

OBJS := $(OBJ_SERIAL)

NOW := $(shell date -u +%s)

all: clean $(OBJS) main.o
	$(CC) $(OBJS) main.o -O3 -o kdtree

test: testcompile
	export MALLOC_TRACE=mtrace/mtrace.$(NOW); ./$@
	$(RM) $@
	mtrace mtrace/mtrace.$(NOW)

testcompile: TESTFLAG := -DKDTREE_TEST
testcompile: clean $(OBJS) test.o
	$(CC) $(OBJS) test.o -DDEBUG -O0 -o test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(TESTFLAG)

clean:
	$(RM) $(OBJS) main.o test.o
