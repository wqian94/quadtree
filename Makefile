CC := gcc
CFLAGS := -std=gnu99 -g
RM := rm

HEADERS := \
	types.h \
	Point.h \
	Quadtree.h

OBJS := \
	serial/SerialSkipQuadtree.o \
	Point.o

NOW := $(shell date -u +%s)

all: $(OBJS) main.o
	$(CC) $(OBJS) main.o -O3 -o quadtree

test: $(OBJS) test.o
	$(CC) $(OBJS) test.o -DDEBUG -O0 -o $@
	export MALLOC_TRACE=mtrace/mtrace.$(NOW); ./$@
	$(RM) $@
	mtrace mtrace/mtrace.$(NOW)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) main.o test.o
