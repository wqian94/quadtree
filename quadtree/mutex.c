#include "./mutex.h"

volatile int64_t CAS(volatile int64_t * addr, int64_t expected_value, int64_t new_value) {
    return CAS64(addr, expected_value, new_value);
}

void Mutex_init(Mutex *m) {
    m->counter = 0;
}

void Mutex_lock(Mutex *m) {
    while (true) {
        if (m->counter == 0)
            if (CAS(&m->counter, 0, 1) == 0)
                return;
        CPU_RELAX
    }
}

void Mutex_unlock(Mutex *m) {
    if (m->counter == 0) {
        printf("Attempted to unlock an unlocked lock %p\n", m);
        abort();
    }
    m->counter = 0;
}
