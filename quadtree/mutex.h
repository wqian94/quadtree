#ifndef MUTEX_H
#define MUTEX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./util.h"

#define CPU_RELAX asm volatile("pause\n": : :"memory");

#define CAS64(m,c,s)                                            \
  ({ int64_t _x = (c);                                          \
     __asm__ __volatile (                                       \
       "lock; cmpxchgq %1,%2;"                                  \
       : "+a" (_x)                                              \
       : "r" ((int64_t)(s)), "m" (*(volatile int64_t *)(m))     \
       : "cc", "memory") ;                                      \
   _x; })

volatile int64_t CAS(volatile int64_t * addr, int64_t expected_value, int64_t new_value);

typedef struct {
    volatile uint64_t counter;
} Mutex;

void Mutex_init(Mutex *m);
void Mutex_lock(Mutex *m);
void Mutex_unlock(Mutex *m);

#endif
